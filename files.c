#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "datatypes.h"
#include "dir.h"
#include "paths.h"
#define LINELEN 128

dir_t *pass, *fail, *extra;
char *dirs, *src, *dst;
unsigned int *count;

void listdir(char *name) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) return;

    while ((entry = readdir(dir))) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
        if (entry->d_type == DT_DIR) {
            if (!strcmp(entry->d_name, ".") ||
                    !strcmp(entry->d_name, "..")) continue;

            listdir(path);
        } else {
            // if file is src or dst continue
            if (!(strcmp(entry->d_name, src) && strcmp(entry->d_name, dst))) continue;

            // if file not in hashfile, add path to dir
            char *short_path = get_relative_path(dirs, path);
            if (path_exists(pass, short_path) == 0
                    && path_exists(fail, short_path) == 0) {
                char copy[strlen(short_path) + 1];
                strcpy(copy, short_path);
                add_path_to_dir(copy, extra);
                (*count)++;
            }
        }
    }

    closedir(dir);
}

// iterate through directories, path is absolute path
void file_iterator(char *dirl, char *srcl, char *dstl,
        dir_t *passl, dir_t *faill, dir_t *extral, unsigned int *countl) {
    pass = passl;
    fail = faill;
    extra = extral;
    dirs = dirl;
    src = srcl;
    dst = dstl;
    count = countl;

    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(dirl))) {
        printf("Something went wrong while reading the directory\n");
        exit(-4);
    }

    while ((entry = readdir(dir))) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dirl, entry->d_name);
        if (entry->d_type == DT_DIR) {
            if (!strcmp(entry->d_name, ".") ||
                    !strcmp(entry->d_name, "..")) continue;

            listdir(dirl);
        } else {
            // if file is src or dst continue
            if (!(strcmp(entry->d_name, src) && strcmp(entry->d_name, dst))) continue;

            // if file not in hashfile, add path to dir
            char *short_path = get_relative_path(dirs, path);
            if (path_exists(pass, short_path) == 0
                    && path_exists(fail, short_path) == 0) {
                char copy[strlen(short_path) + 1];
                strcpy(copy, short_path);
                add_path_to_dir(copy, extra);
                (*count)++;
            }
        }
    }

    closedir(dir);
}

// mallocs a line read from file
char *get_line(FILE *f) {
    char buffer[LINELEN];
    char *line = malloc(LINELEN * sizeof(char));
    line[0] = '\0';

    // collects chunks of bytes until encounter linebreak
    while(fgets(buffer, LINELEN, f) != NULL) {
        strcat(line, buffer);
        if (line[strlen(line) - 1] == '\n') return line;
        line = realloc(line, (strlen(line) + LINELEN) * sizeof(char));
    }

    free(line);
    return NULL;
}

// checks if path exists as file: 1 or dir: 0
int check_exists(char *path, int file) {
    struct stat s;
    if (stat(path, &s)) {
        return 0;
    }

    return file ? S_ISREG(s.st_mode) : S_ISDIR(s.st_mode);
}

// writes to buffer and flushes to file
// if successful returns 1
void write_to_file(FILE *f, char *format, char *line) {
    fprintf(f, format, line);
    fflush(f);
}
