#include <string.h>
#include <limits.h>

// concats dir and file and copies into file
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
