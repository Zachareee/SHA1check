#include <ftw.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    return ftw(dir, &file_iterator, 1);
}


FILE *open_file(char *dir, char *src, char *opt) {
    // constructs path to hashfile
    char hashfile[strlen(dir) + strlen(src) + 1];
    strcpy(hashfile, dir);
    strcat(hashfile, src);

    // opens file buffer and frees malloc
    FILE *f = fopen(hashfile, opt);
    return f;
}

// mallocs an absolute path
void get_real_path(char **dir) {
    char *path = malloc(PATH_MAX * sizeof(char));
    realpath(*dir, path);

    // adds an additional slash
    int len = strlen(path);
    path[len] = '/';
    path[len + 1] = '\0';

    // assigns back to directory
    *dir = path;
}

char *get_line(FILE *f) {
    char buffer[LINELEN];
    char *line = malloc(LINELEN * sizeof(char));
    line[0] = '\0';

    while(fgets(buffer, LINELEN, f) != NULL) {
        printf("%s\n", line);
        strcat(line, buffer);
        if (line[strlen(line) - 1] == '\n') return line;
        line = realloc(line, (strlen(line) + LINELEN) * sizeof(char));
    }

    printf("This line does not end with a linebreak: %s", line);
    free(line);
    return NULL;
}
