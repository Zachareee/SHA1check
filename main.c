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

    if (regex_init()) {
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
    // 0: OK, 1: MISSING, 2: FAILED, 3: EXTRA
    int arr[4] = {0};

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
        if (!c) {
            state = "OK";
            arr[0]++;
        } else {
            add_path_to_dir(ptr, failed);
            state = "FAILED";
            arr[2]++;
        }

        if (c == -2) {
            add_path_to_dir(ptr, missing);
            state = "MISSING";
            arr[1]++;
        }

        write_to_file(checkfile, "%s: ", ptr);
        write_to_file(checkfile, "%s\n", state);
    }

    fclose(hashfile);

    for (int i = 0; i < file_count; i++) {
        if (!files[i].checked) {
            add_path_to_dir(get_relative_path(dir, files[i].name), extra);
            arr[1]++;
        }
    }

    write_to_file(checkfile, "\n%d files OK\n\n", arr[0]);

    write_to_file(checkfile, "%d files were not found in the hashfile:\n", arr[3]);
    write_dir_to_file(extra, 0, checkfile);

    write_to_file(checkfile, "\n%d files failed hashsum checks:\n", arr[2]);
    write_dir_to_file(failed, 0, checkfile);

    write_to_file(checkfile, "\n%d files could not be found:\n", arr[1]);
    write_dir_to_file(missing, 0, checkfile);

    fclose(checkfile);

    free_dir(missing);
    free_dir(failed);
    free_dir(extra);
    free_all();
}
