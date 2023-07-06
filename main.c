#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "compare.h"
#include "files.h"
#include "hashing.h"

int main(int argc, char **argv) {
    char *src;
    char *dst;
    char *dir;

    // if parse_args returns -1 exit program
    int exit = parse_args(argc, argv, &src, &dst, &dir);
    if (exit) return exit;

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
    printf("%s", dir);
    FILE *hashfile = open_file(dir, src, "r");
    get_line(hashfile);
    printf("File count total: %d\n", file_count);
}
