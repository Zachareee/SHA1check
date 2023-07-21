#include <string.h>
#include <limits.h>
#include <stdio.h>

// concats dir and file and copies into file
void concat_path(char *dir, char *file) {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/%s", dir, file);
    strcpy(file, path);
}

char *get_relative_path(char *dir, char *file) {
    // +1 to account for last slash before the relative path
    return file + strlen(dir) + 1;
}
