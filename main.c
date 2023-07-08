#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "compare.h"
#include "dir.h"
#include "files.h"
#include "hashing.h"
#include "paths.h"

int main(int argc, char **argv) {
    char src[PATH_MAX];
    char dst[PATH_MAX];
    char dir[PATH_MAX];

    // if parse_args returns -1 exit program
    int exit;
    if ((exit = parse_args(argc, argv, src, dst, dir))) return exit;

    // checks if file exists
    concat_path(dir, src);
    concat_path(dir, dst);
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
        printf("Something went wrong while detecting the directory\n");
        return exit;
    }

    FILE *hashfile = fopen(src, "r");
    FILE *checkfile = fopen(dst, "w");

    char *line = get_line(hashfile);
    char ptr[PATH_MAX];
    while (line) {
        strcpy(ptr, line);
        free(line);
        int c = compare(dir, ptr);

        if (c != -1) {
            if (!write_to_file(checkfile, ptr)
                    || !write_to_file(checkfile, ": ")
                    || !write_to_file(checkfile,
                        c ? (c == -2 ? "MISSING\n" : "FAILED\n") : "OK\n")) {
                printf("Something went wrong while writing to %s\n", dst);
                return -3;
            }
        }

        if (!c) mark_file(dir, ptr);
        line = get_line(hashfile);
    }

    fclose(checkfile);
    fclose(hashfile);

    dir_t *current;
    current = calloc(1, sizeof(dir_t));
    path_to_dir(files[0].name, current);
    dir_iterator(current);
    for (int i = 0; i < file_count; i++) {
        //if (files[i].checked) printf("Debug: %s checked\n", files[i].name);
        free(files[i].name);
    }

    free(files);

    // printf("File count total: %d\n", file_count);
}
