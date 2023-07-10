#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "datatypes.h"
#include "dir.h"
#include "paths.h"
#define LINELEN 128

char *files_dir, *files_src, *files_dst;
dir_t *files_pass, *files_fail, *files_extra;
unsigned int *num;

// iterate through directories, path is absolute path
int file_iterator(const char *path, const struct stat *sb, int type) {
    // return if encounter directory
    if (type != FTW_F) return 0;

    // if file is src or dst return
    if (!(strcmp(path, files_src) && strcmp(path, files_dst))) return 0;

    // if file not in hashfile, add path to dir
    char *short_path = get_relative_path(files_dir, (char *)path);
    if (path_exists(files_pass, short_path) == 0
            && path_exists(files_fail, short_path) == 0) {
        char copy[strlen(short_path) + 1];
        strcpy(copy, short_path);
        add_path_to_dir(copy, files_extra);
        (*num)++;
    }

    return 0;
}

void loop_files(char *dir, char *src, char *dst,
        dir_t *pass, dir_t *fail, dir_t *extra, unsigned int *count) {
    files_dir = dir;
    files_src = src;
    files_dst = dst;
    files_pass = pass;
    files_fail = fail;
    files_extra = extra;
    num = count;

    int status = ftw(dir, &file_iterator, 1);
    if (status) {
        printf("Something went wrong while reading the directory\n");
        exit(status);
    }
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
