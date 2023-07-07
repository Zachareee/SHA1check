#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// mallocs an absolute path
char *get_abs_path(char *dir) {
    char *path = malloc(PATH_MAX * sizeof(char));
    realpath(dir, path);

    // adds an additional slash
    // int len = strlen(path);
    // path[len] = '/';
    // path[len + 1] = '\0';

    // assigns back to directory
    return path;
}

void concat_path(char *dir, char *file) {
    char ptr[PATH_MAX];
    *ptr = '\0';
    strcat(ptr, dir);
    strcat(ptr, "/");
    strcat(ptr, file);
    strcpy(file, ptr);
}

char *get_relative_path(char *dir, char *file) {
    // +1 to account for last slash before the relative path
    return file + strlen(dir) + 1;
}
