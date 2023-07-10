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

dir_t *passed;
dir_t *missing;
dir_t *failed;
dir_t *extra;

void free_all() {
    if (passed) free_dir(passed);
    if (missing) free_dir(missing);
    if (failed) free_dir(failed);
    if (extra) free_dir(extra);
    free_regex();
}

int main(int argc, char **argv) {
    atexit(free_all);
    char src[PATH_MAX];
    char dst[PATH_MAX];
    char dir[PATH_MAX];

    parse_args(argc, argv, src, dst, dir);

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

    regex_init();

    FILE *hashfile = fopen(src, "r");
    FILE *checkfile = fopen(dst, "w");

    char *line = get_line(hashfile);
    char ptr[PATH_MAX];
    char *state;
    // 0: OK, 1: MISSING, 2: FAILED, 3: EXTRA
    unsigned int arr[4] = {0};

    passed = calloc(1, sizeof(dir_t));
    missing = calloc(1, sizeof(dir_t));
    failed = calloc(1, sizeof(dir_t));
    extra = calloc(1, sizeof(dir_t));

    while (line) {
        strcpy(ptr, line);
        free(line);
        int c = compare(dir, ptr);

        line = get_line(hashfile);

        if (c == -1) continue;

        if (!c) {
            add_path_to_dir(ptr, passed);
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

    loop_files(dir, src, dst, passed, failed, extra, arr + 3);

    // creates a char array which can hold the number of files as text
    char length[11] = {0};

    snprintf(length, 10, "%d", arr[0]);
    write_to_file(checkfile, "\n%s files OK\n", length);

    snprintf(length, 10, "%d", arr[3]);
    write_to_file(checkfile, "\n%s files were not found in the hashfile:\n", length);
    write_dir_to_file(extra, 0, checkfile);

    snprintf(length, 10, "%d", arr[2]);
    write_to_file(checkfile, "\n%s files failed hashsum checks:\n", length);
    write_dir_to_file(failed, 0, checkfile);

    snprintf(length, 10, "%d", arr[1]);
    write_to_file(checkfile, "\n%s files could not be found:\n", length);
    write_dir_to_file(missing, 0, checkfile);

    fclose(checkfile);
}
