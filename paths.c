#include <stdlib.h>
#include <string.h>
#include <limits.h>

// mallocs an absolute path
void get_abs_path(char **dir) {
    char *path = malloc(PATH_MAX * sizeof(char));
    realpath(*dir, path);

    // adds an additional slash
    int len = strlen(path);
    path[len] = '/';
    path[len + 1] = '\0';

    // assigns back to directory
    *dir = path;
}

void concat_path(char *dir, char **file) {
    char *ptr = malloc((strlen(dir) + strlen(*file) + 1) * sizeof(char));
    *ptr = '\0';
    strcat(ptr, dir);
    strcat(ptr, *file);
    ptr[strlen(ptr)] = '\0';
    *file = ptr;
}
