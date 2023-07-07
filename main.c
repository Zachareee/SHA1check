#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "compare.h"
#include "files.h"
#include "hashing.h"
#include "paths.h"

int main(int argc, char **argv) {
    char *src;
    char *dst;
    char *dir;

    // if parse_args returns -1 exit program
    int exit;
    if ((exit = parse_args(argc, argv, &src, &dst, &dir))) return exit;

    // checks if file exists
    get_abs_path(&dir);
    concat_path(dir, &src);
    concat_path(dir, &dst);
    if (!(check_exists(src, 1) && check_exists(dir, 0))) {
        printf("Check your inputs\n");
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
    free(src);
    free(dst);
    free(dir);
}
