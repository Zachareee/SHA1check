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
}

int main(int argc, char **argv) {
    atexit(free_all);
    char src[PATH_MAX];
    char dst[PATH_MAX];
    char dir[PATH_MAX];

    parse_args(argc, argv, src, dst, dir);
    init_path(dir);

    // checks if file exists
    if (!check_exists(dir, 0)) {
        printf("%s is not a directory\n", dir);
        return -1;
    }

    if (!check_exists(src, 1)) {
        printf("%s is not a file\n", src);
        return -1;
    }

    FILE *hashfile = fopen(src, "r");

    int version = 0;
    char *line;
    while ((line = get_line(hashfile))) {
        version = get_hash_ver(line);
        free(line);
        if (version) goto ver;
    }

    printf("Could not find hashsum version, exiting...\n");
    return -5;

    ver:

    fprintf(stderr, "Version %d hashfile detected\n", version);
    fseek(hashfile, 0, SEEK_SET);
    FILE *checkfile = fopen(dst, "w");

    char ptr[PATH_MAX];
    char *state;
    // 0: OK, 1: MISSING, 2: FAILED, 3: EXTRA

    passed = calloc(1, sizeof(dir_t));
    missing = calloc(1, sizeof(dir_t));
    failed = calloc(1, sizeof(dir_t));
    extra = calloc(1, sizeof(dir_t));

    while ((line = get_line(hashfile))) {
        strcpy(ptr, line);
        free(line);
        int c = compare(dir, ptr, version);

        if (c == -1) continue;

        write_to_file(checkfile, "%s: ", ptr);

        switch (c) {
            case 0:
                add_path_to_dir(ptr, passed);
                state = "OK";
                break;
            case -2:
                add_path_to_dir(ptr, missing);
                state = "MISSING";
                break;
            default:
                add_path_to_dir(ptr, failed);
                state = "FAILED";
        }

        write_to_file(checkfile, "%s\n", state);
    }

    fclose(hashfile);

    unsigned int not_found = 0;
    file_iterator(dir, src, dst, passed, failed, extra, &not_found);

    // creates a char array which can hold the number of files as text
    char length[11] = {0};

    snprintf(length, 10, "%d", count_files_in_dir(passed));
    write_to_file(checkfile, "\n%s files OK\n", length);

    snprintf(length, 10, "%d", count_files_in_dir(extra));
    write_to_file(checkfile, "\n%s files were not found in the hashfile:\n", length);
    write_dir_to_file(extra, 0, checkfile);

    int failed_count = count_files_in_dir(failed);
    snprintf(length, 10, "%d", failed_count);
    write_to_file(checkfile, "\n%s files failed hashsum checks:\n", length);
    write_dir_to_file(failed, 0, checkfile);

    int missing_count = count_files_in_dir(missing);
    snprintf(length, 10, "%d", missing_count);
    write_to_file(checkfile, "\n%s files could not be found:\n", length);
    write_dir_to_file(missing, 0, checkfile);

    fclose(checkfile);

    printf("\n%d files failed hashsum checks, %d files could not be found\n", failed_count, missing_count);
}
