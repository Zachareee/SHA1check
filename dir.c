#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "datatypes.h"

// creates directory with name and appends to dir
void append_dir(dir_t *dir, char *name) {
    dir->folder = realloc(dir->folder, ++(dir->num) * sizeof(dir_t *));
    dir_t *new_dir = malloc(sizeof(dir_t));
    new_dir->name = name;
    new_dir->num = 0;
    new_dir->folder = NULL;
    dir->folder[dir->num - 1] = new_dir;
}

// splits a path string by / and creates a dir_t form listing
void path_to_dir(char *path, dir_t *dir) {
    char *token = strtok(path, "/");
    while (token) {
        append_dir(dir, token);
        dir = dir->folder[dir->num - 1];
        token = strtok(NULL, "/");
    }
    printf("\n");
}

// iterates through a dir_t
void dir_iterator(dir_t *dir) {
    while (dir && dir->folder) {
        printf("%s\n", dir->name);
        dir_t *temp = dir->folder[0];
        free(dir);
        free(dir->folder);
        dir = temp;
    }
    free(dir);
}
