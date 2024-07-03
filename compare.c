#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "files.h"
#include "hashing.h"
#include "paths.h"
#ifdef _WIN32
#include <windows.h>
#endif

#define VER1HASHLENGTH 40
#define VER2HASHLENGTH 23

int is_hex(char c) {
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f');
}

// homemade regex
int matcher(char *str, int ver, int *range) {
    int len = strlen(str);
    if (ver == 1) {
        for (int i = 0; i < len; i++) {
            int j = i;
            for (; j < len; j++) {
                if (!is_hex(str[j]))
                    break;
            }

            if (j - i == VER1HASHLENGTH) {
                if (range) {
                    range[0] = i;
                    range[1] = j;
                }
                return 0;
            }
        }
    } else {
        for (int i = 0; i < len; i++) {
            int j = 0;
            for (; i + j < len && j < 10; j++) {
                if (!is_hex(str[i + j]))
                    break;
            }

            // condition to allow size 0 files
            if (j == 1 && str[i] == '0' && (str[i + 1] == '\r' || str[i + 1] == '\n')) {
                range[0] = i;
                return 0;
            }

            if (j != 10)
                continue;

            for (; i + j < len && j < 13; j++) {
                if (str[i + j] != '*')
                    break;
            }

            if (j != 13)
                continue;

            for (; i + j < len && j < VER2HASHLENGTH; j++) {
                if (!is_hex(str[i + j]))
                    break;
            }

            if (j != VER2HASHLENGTH)
                continue;
            if (range) {
                range[0] = i;
                range[1] = i + VER2HASHLENGTH;
            }

            return 0;
        }
    }
    return 1;
}

// if full hash given, return 1
// if partial hash given in the form of reghex2, returns 2
// if unknown, return 0
int get_hash_ver(char *line) {
    if (!matcher(line, 1, NULL))
        return 1;
    if (!matcher(line, 2, NULL))
        return 2;
    return 0;
}

// trim trailing whitespaces in the line
void trim(char *line, int offset) {
    for (int i = offset - 1; line[i] == ' ' || line[i] == '\t'; i--) {
        line[i] = 0;
    }
}

// obfuscates hash
void obf_hash(char *line, char *result) {
    strncpy(result, line, 10);
    strncpy(result + 10, "***", 3);
    strncpy(result + 13, line + 30, 10);
}

// compare hash of file against recorded hash
// returns -1 if no hash found, -2 if file not found
// 0 if hashsum equal and positive if strcmp fails
int compare(char *dir, char *line, int ver) {
    int limit;

    if (ver == 1) {
        limit = VER1HASHLENGTH;
    } else {
        limit = VER2HASHLENGTH;
    }

    int range[2] = {0};
    // find hexstring and copy to hex, return -1 if no matches
    int match = matcher(line, ver, range);
    if (match)
        return -1;

    char hex[limit + 1];
    hex[limit] = 0;
    strncpy(hex, line + range[0], limit);

    // trim line to find file
    trim(line, range[0]);

    // copy to path and concat
    char path[PATH_MAX];
    strcpy(path, line);
    concat_path(dir, path);
    if (!check_exists(path, 1))
        return -2;

    fprintf(stderr, "Checking %s...", get_relative_path(path));

    // construct file_struct to give to hash function
    unsigned long long fSize;
#ifdef _WIN32
    HANDLE fileH = CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileH == INVALID_HANDLE_VALUE)
        return -1;

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(fileH, &fileSize)) {
        CloseHandle(fileH);
        return -1;
    }
    CloseHandle(fileH);
    fSize = fileSize.QuadPart;
#else
    struct stat s;
    stat(path, &s);
    fSize = s.st_size;
#endif

    char *hash_value = hash(path, fSize);

    long result;
    if (ver == 1) {
        result = (long)strcmp(hash_value, hex);
        // printf("\nExpected: %s\nCurrent:  %s\n", hex, hash_value);
    } else {
        // condition if file sizes are both 0
        if (strcmp("0", hex) && !fSize) {
            fprintf(stderr, "OK\n");
            return 0;
        }

        long size;
        int stat = sscanf(line + range[1], "%ld", &size);
        if (!stat) {
            printf("Error reading size value\n");
            exit(-6);
        }

        char temp[24] = {0};
        obf_hash(hash_value, temp);
        result = (long)strcmp(temp, hex);
        // printf("\nExpected: %s\nCurrent:  %s\n", hex, temp);
        result |= fSize - size;
    }

    fprintf(stderr, "OK\n");

    // free malloc-ed variables
    free(hash_value);
    return !!result;
}
