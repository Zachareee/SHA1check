#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "compare.h"
#include "files.h"
#include "hashing.h"
#include "paths.h"

char src[PATH_MAX];
char dst[PATH_MAX];
char dir[PATH_MAX];

int main(int argc, char **argv) {
    // if parse_args returns -1 exit program
    int exit;
    if ((exit = parse_args(argc, argv, &src, &dst, &dir))) return exit;

    // checks if file exists
    concat_path(dir, &src);
    concat_path(dir, &dst);
    if (!check_exists(dir, 0)) {
        printf("%s is not a directory\n", dir);
        return -1;
    }

    if (!check_exists(src, 1)) {
        printf("%s is not a file\n", src);
        return -1;
    }

    if (comp_init()) {
        printf("Something went wrong while initialising pattern matcher\n");
        return -2;
    }

    if ((exit = loop_files(dir))) {
        printf("Something went wrong while reading the files\n");
        return exit;
    }

    FILE *hashfile = fopen(src, "r");

    char *line = get_line(hashfile);
    while (line) {
        int c = compare(dir, line);
        if (!c) mark_file(dir, line);
        line = get_line(hashfile);
    }

    fclose(hashfile);

    // type cast because for some reason it becomes char **
    // instead of file_struct*
    file_struct *ptr = (file_struct *)files;
    for (int i = 0; i < file_count; i++) {
        if (ptr[i].checked) printf("Debug: %s checked\n", ptr[i].name);
        free(ptr[i].name);
    }

    free(ptr);

    // printf("File count total: %d\n", file_count);
}
