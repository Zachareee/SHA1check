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

// if matching path is found, return the idx
// return -1 if not found
int path_exists(dir_t *dir, char *name) {
    char *folder_name;
    for (int i = 0; i < dir->num; i++) {
        folder_name = dir->folder[i]->name;
        if (folder_name && !strcmp(name, folder_name)) return i;
    }
    return -1;
}

// splits a path string by / and creates a dir_t form listing
void add_path_to_dir(char *path, dir_t *dir) {
    char *token = strtok(path, "/");
    char *copy;
    int idx;
    while (token) {
        copy = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(copy, token);
        if ((idx = path_exists(dir, token)) == -1) {
            append_dir(dir, copy);
            dir = dir->folder[dir->num - 1];
            token = strtok(NULL, "/");
        } else {
            dir = dir->folder[idx];
            token = strtok(NULL, "/");
        }
    }
}

// iterates through a dir_t
void free_dir(dir_t *dir) {
    // while (dir && dir->folder) {
    //     printf("%s\n", dir->name);
    //     dir_t *temp = dir->folder[0];
    //     free(dir);
    //     free(dir->folder);
    //     dir = temp;
    // }

    // free(dir);
    if (!dir->num) {
        free(dir->name);
        return;
    }

    for (int i = 0; i < dir->num; i++) {
        free_dir(dir->folder[i]);
    }

    free(dir->name);
    free(dir);
}

void print_dir(dir_t *dir, int level) {
    for (int i = 0; i < level; i++) printf("|--");
    if (level) printf("%s %d\n", dir->name, level);
    for (int i = 0; i < dir->num; i++) {
        print_dir(dir->folder[i], level + 1);
    }
}
