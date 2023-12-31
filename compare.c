#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "files.h"
#include "hashing.h"
#include "paths.h"

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
                if (!is_hex(str[j])) break;
            }

            if (j - i == 40) {
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
                if (!is_hex(str[i + j])) break;
            }

            if (j != 10) continue;

            for (; i + j < len && j < 13; j++) {
                if (str[i + j] != '*') break;
            }

            if (j != 13) continue;

            for (; i + j < len && j < 23; j++) {
                if (!is_hex(str[i + j])) break;
            }

            if (j != 23) continue;
            if (range) {
                range[0] = i;
                range[1] = i + 23;
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
    if (!matcher(line, 1, NULL)) return 1;
    if (!matcher(line, 2, NULL)) return 2;
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
        limit = 40;
    } else {
        limit = 23;
    }

    int range[2] = {0};
    // find hexstring and copy to hex, return -1 if no matches
    int match = matcher(line, ver, range);
    if (match) return -1;

    char hex[limit + 1];
    hex[limit] = 0;
    strncpy(hex, line + range[0], limit);

    // trim line to find file
    trim(line, range[0]);

    // copy to path and concat
    char path[PATH_MAX];
    strcpy(path, line);
    concat_path(dir, path);
    if (!check_exists(path, 1)) return -2;

    fprintf(stderr, "Checking %s...", get_relative_path(path));

    // construct file_struct to give to hash function
    struct stat s;
    stat(path, &s);

    char *hash_value = hash(path, s.st_size);

    long result;
    if (ver == 1) {
        result = (long) strcmp(hash_value, hex);
        // printf("\nExpected: %s\nCurrent:  %s\n", hex, hash_value);
    } else {
        long size;
        int stat = sscanf(line + range[1], "%ld", &size);
        if (!stat) {
            printf("Error reading size value\n");
            exit(-6);
        }

        char temp[24] = {0};
        obf_hash(hash_value, temp);
        result = (long) strcmp(temp, hex);
        // printf("\nExpected: %s\nCurrent:  %s\n", hex, temp);
        result |= s.st_size - size;
    }

    fprintf(stderr, "OK\n");

    // free malloc-ed variables
    free(hash_value);
    return !!result;
}
