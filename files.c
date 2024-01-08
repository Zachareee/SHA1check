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

// mallocs a line read from file
char *get_line(FILE *f) {
    char buffer[LINELEN];
    char *line = malloc(LINELEN * sizeof(char));
    line[0] = '\0';

    // collects chunks of bytes until encounter linebreak
    while(fgets(buffer, LINELEN, f) != NULL) {
        strcat(line, buffer);
        if (strlen(buffer) < LINELEN - 1) return line;
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
    int len = fprintf(f, format, line);
    if (!len) {
        printf("Something went wrong while writing to the file");
        exit(-3);
    }
    fflush(f);
}

unsigned long write_dir_and_filecount(FILE *f, dir_t *dir, char *string) {
    unsigned long count = 0;
    long pos = ftell(f);
    write_dir_to_file(dir, 0, f, &count);
    fseek(f, pos, SEEK_SET);
    char length[11] = {0};
    snprintf(length, 10, "%lu", count);
    write_to_file(f, string, length);
    fseek(f, 0, SEEK_END);
    return count;
}