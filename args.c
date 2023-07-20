#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "paths.h"
#include "version.h"

void parse_args(int argc, char **argv, char *src,
        char *dst, char *dir) {
    // if no arguments return
    if (argc < 2) {
        printf("Usage: %s DIRECTORY\n", argv[0]);
        exit(-1);
    }

    /* if directory is second arg set var
       and move optind */
    char *directory = NULL;
    if (argv[1][0] != '-') {
        directory = argv[1];
        optind = 2;
    }

    // initialise opt and set defaults
    int opt;
    char *source = "hashes.sha1";
    char *destination = "checksum.txt";

    // loop to get values for options
    // which is -s and -d
    while ((opt = getopt(argc, argv, ":s:d:v-")) != -1) {
        switch (opt) {
            // return if value is missing
            case ':':
                printf("Missing value for -%c\n", argv[optind - 2][1]);
                exit(-1);
                // return if unknown option
            case '?':
                printf("Unknown option: -%c\n", optopt);
                exit(-1);
            case 'v':
                printf("SHA1sum version: %s\n",version);
                exit(0);
            case 'd':
            case 's':
                // return if value starts with ':'
                if (optarg[0] == '-') {
                    printf("Invalid value for -%c: %s\n",
                            argv[optind - 2][1], optarg);
                    exit(-1);
                }

                // assign values
                switch (opt) {
                    case 'd':
                        destination = optarg;
                        break;
                    case 's':
                        source = optarg;
                        break;
                }
        }
    }

    // if directory not set yet
    if (directory == NULL) {

        // if directory not in arguments
        if (optind == argc) {
            printf("Usage: %s DIRECTORY\n", argv[0]);
            exit(-1);
        }

        // assigns directory using optind
        // optind = next argv index
        else directory = argv[optind];
    }

    // realpath(directory, dir); // uncomment for testing
    strcpy(dir, directory); // uncomment for production

    if (source[0] == ':') strcpy(src, source + 1);
    else {
        strcpy(src, source);
        concat_path(directory, src);
    }

    if (destination[0] == ':') strcpy(dst, destination + 1);
    else {
        strcpy(dst, destination);
        concat_path(directory, dst);
    }
}
