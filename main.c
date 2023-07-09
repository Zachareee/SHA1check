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
    char *state;

    dir_t *missing = calloc(1, sizeof(dir_t));
    dir_t *failed = calloc(1, sizeof(dir_t));
    dir_t *extra = calloc(1, sizeof(dir_t));

    while (line) {
        strcpy(ptr, line);
        free(line);
        int c = compare(dir, ptr);

        line = get_line(hashfile);

        if (c == -1) continue;

        mark_file(dir, ptr);
        if (!c) state = "OK\n";
        else {
            add_path_to_dir(ptr, failed);
            state = "FAILED\n";
        }

        if (c == -2) {
            add_path_to_dir(ptr, missing);
            state = "MISSING\n";
        }

        strcat(ptr, ": ");
        strcat(ptr, state);
        if (!write_to_file(checkfile, ptr)) {
            return -3;
        }
    }

    fclose(checkfile);
    fclose(hashfile);

    for (int i = 0; i < file_count; i++) {
        if (!files[i].checked)
            add_path_to_dir(get_relative_path(dir, files[i].name), extra);
    }

    printf("Missing:\n");
    print_dir(missing, 0);
    printf("Failed:\n");
    print_dir(failed, 0);
    printf("Extra:\n");
    print_dir(extra, 0);

    free_dir(missing);
    free_dir(failed);
    free_dir(extra);
    free_files();

    // printf("File count total: %d\n", file_count);
}
