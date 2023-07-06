#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **files = NULL;
int file_count = 0;

int file_iterator(const char *path, const struct stat *sb, int type) {
    if (type != FTW_F) return 0;
    files = realloc(files, (file_count + 1) * sizeof(char*));
    files[file_count] = malloc(sizeof(char) * strlen(path));
    strcpy(files[file_count++], path);
    printf("%s, %lld\n", files[file_count - 1], sb->st_size);
    return 0;
}

int loop_files(char *dir) {
    return ftw(dir, &file_iterator, 1);
}
