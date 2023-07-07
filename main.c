#include <stdio.h>
#include <stdlib.h>

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

    if ((exit = loop_files(dir))) {
        printf("Something went wrong while reading the files\n");
        return exit;
    }

    // type cast because for some reason it becomes char **
    // instead of file_struct*
    file_struct *ptr = (file_struct *)files;
    for (int i = 0; i < file_count; i++) {
        char *result = hash(ptr[i]);
        free(result);
        free(ptr[i].name);
    }

    free(ptr);
    FILE *hashfile = open_file(dir, src, "r");

    // DEBUG
    char *line = get_line(hashfile);

    compare(line);
    printf("%s", line);
    free(line);

    printf("File count total: %d\n", file_count);
}
