#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "datatypes.h"
#include "dir.h"
#include "paths.h"
#define LINELEN 256

// mallocs a line read from file
char *get_line(FILE *f) {
    char buffer[LINELEN];
    char *line = malloc(LINELEN * sizeof(char));
    line[0] = '\0';

    // collects chunks of bytes until encounter linebreak
    while (fgets(buffer, LINELEN, f) != NULL) {
        strcat(line, buffer);
        if (strlen(buffer) < LINELEN - 1)
            return line;
        line = realloc(line, (strlen(line) + LINELEN) * sizeof(char));
    }

    free(line);
    return NULL;
}

// checks if path exists as file: true or dir: false
bool check_exists(char *path, bool file) {
    struct stat s;
    if (stat(path, &s)) {
        return 0;
    }

    return file ? S_ISREG(s.st_mode) : S_ISDIR(s.st_mode);
}

bool is_a_file(char *path) {
    return check_exists(path, true);
}

bool is_a_directory(char *path) {
    return check_exists(path, false);
}

// writes to buffer and flushes to file
void write_to_file(FILE *f, char *format, char *line) {
    int len = fprintf(f, format, line);
    if (!len) {
        printf("Something went wrong while writing to the file");
        exit(-3);
    }
    fflush(f);
}

// writes file count of directory with message, write_dir = true writes whole directory
int write_file_count(dir_t *directory, char *message, FILE *checkfile, bool write_dir) {
    unsigned long num = count_files_in_dir(directory);
    char buffer[11] = {0};
    snprintf(buffer, 10, "%lu", num);
    write_to_file(checkfile, message, buffer);
    if (write_dir)
        write_dir_to_file(directory, 0, checkfile);
    return num;
}
