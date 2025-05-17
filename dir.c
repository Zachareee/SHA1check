#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes.h"
#include "files.h"

#define PATH_SEP "/"
// creates directory with name and appends to dir
void append_dir(dir_t *dir, char *name) {
    dir->folder = realloc(dir->folder, (dir->num + 1) * sizeof(dir_t *));
    dir_t *new_dir = calloc(1, sizeof(dir_t));
    new_dir->name = name;
    dir->folder[dir->num++] = new_dir;
}

// if matching folder is found, return the idx
// return -1 if not found
int folder_exists(dir_t *dir, char *name) {
    char *folder_name;
    for (int i = 0; i < dir->num; i++) {
        folder_name = dir->folder[i]->name;
        if (folder_name && !strcmp(name, folder_name)) return i;
    }

    return -1;
}

// if matching path is found, return 1
// return 0 if not found
int path_exists(dir_t *dir, const char *path) {
    char copy[strlen(path) + 1];
    strcpy(copy, path);

    char *token = strtok(copy, PATH_SEP);
    int idx;
    while (token) {
        if ((idx = folder_exists(dir, token)) == -1) return 0;
        dir = dir->folder[idx];
        token = strtok(NULL, PATH_SEP);
    }

    return 1;
}

// splits a path string by / and creates a dir_t form listing
void add_path_to_dir(char *path, dir_t *dir) {
    char *token = strtok(path, PATH_SEP);
    char *copy;
    int idx;

    while (token) {
        // if folder does not exist, append dir and continue
        if ((idx = folder_exists(dir, token)) == -1) {
            copy = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(copy, token);
            append_dir(dir, copy);
            dir = dir->folder[dir->num - 1];
        } else dir = dir->folder[idx];

        token = strtok(NULL, PATH_SEP);
    }
}

// frees a dir_t
void free_dir(dir_t *dir) {
    for (int i = 0; i < dir->num; i++) {
        free_dir(dir->folder[i]);
    }

    free(dir->folder);
    free(dir->name);
    free(dir);
}

// write the whole dir struct to checksum file
void write_dir_to_file(dir_t *dir, int level, FILE *f) {
    for (int i = 0; i < level; i++) write_to_file(f, "%s", "|--");
    if (level) write_to_file(f, "%s\n", dir->name);
    for (int i = 0; i < dir->num; i++) {
        write_dir_to_file(dir->folder[i], level + 1, f);
    }
}

int count_files_in_dir_recurse(dir_t *dir) {
    int count = 0;
    for (int i = 0; i < dir->num; i++) {
        count += count_files_in_dir_recurse(dir->folder[i]);
    }

    return count + !dir->num;
}

int count_files_in_dir(dir_t *dir) {
    int count = 0;
    for (int i = 0; i < dir->num; i++) {
        count += count_files_in_dir_recurse(dir->folder[i]);
    }

    return count;
}