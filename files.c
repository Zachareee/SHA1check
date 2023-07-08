#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "datatypes.h"
#include "paths.h"
#define LINELEN 128

file_struct_t *files = NULL;
int file_count = 0;

// iterate through directories
int file_iterator(const char *path, const struct stat *sb, int type) {
    // return if encounter directory
    if (type != FTW_F) return 0;

    // increase size of array by 1
    files = realloc(files, (file_count + 1) * sizeof(file_struct_t));

    // assign new file in array and increment file counter
    char *name = malloc((sizeof(char) + 1) * strlen(path));
    strcpy(name, path);
    size_t size = sb->st_size;
    file_struct_t file = {name, size, 0};
    files[file_count++] = file;
    return 0;
}

int loop_files(char *dir) {
    return ftw(dir, &file_iterator, 1);
}

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

// checks if path exists as d: dir or f: file
int check_exists(char *path, int file) {
    struct stat s;
    stat(path, &s);
    return file ? S_ISREG(s.st_mode) : S_ISDIR(s.st_mode);
}

// marks a file in the file_struct_t array as checked
void mark_file(char *dir, char *filename) {
    for (int i = 0; i < file_count; i++) {
        if (!strcmp(filename, get_relative_path(dir, files[i].name))) {
            files[i].checked = 1;
            return;
        }
    }
}

int write_to_file(FILE *f, char *line) {
    int count = strlen(line);
    int write = fwrite(line, sizeof(char), count, f);
    return count == write;
}
