#include <ftw.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "datatypes.h"

file_struct *files = NULL;
int file_count = 0;

int file_iterator(const char *path, const struct stat *sb, int type) {
    // return if encounter directory
    if (type != FTW_F) return 0;

    // increase size of array by 1
    files = realloc(files, (file_count + 1) * sizeof(file_struct));

    // assign new file in array and increment file counter
    // file_struct *file = malloc(sizeof(file_struct));
    char *name = malloc((sizeof(char) + 1) * strlen(path));
    strcpy(name, path);
    size_t size = sb->st_size;
    file_struct file = {name, size};
    files[file_count++] = file;
    return 0;
}

int loop_files(char *dir) {
    // force ftw to output full path and get dir_len to cut
    return ftw(dir, &file_iterator, 1);
}


FILE *open_file(char *dir, char *src, char *opt) {
    // constructs path to hashfile
    char *hashfile = malloc(strlen(dir) + strlen(src) + 2);
    strcpy(hashfile, dir);
    strcat(strcat(hashfile, "/"), src);
    printf("%s\n", hashfile);

    // opens file buffer and frees malloc
    FILE *f = fopen(hashfile, opt);
    free(hashfile);
    return f;
}
