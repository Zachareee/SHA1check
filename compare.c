#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "files.h"
#include "hashing.h"
#include "paths.h"
#define LINELEN 128

regex_t reghex;

// init regex sequences
void regex_init() {
    int regh = regcomp(&reghex, "[0-9a-fA-F]{40}", REG_EXTENDED);
    if (regh) {
        printf("Something went wrong while initialising pattern matcher\n");
        exit(-2);
    }
}

// trim trailing whitespaces in the line
void trim(char *line, int offset) {
    for (int i = offset - 1; line[i] == ' ' || line[i] == '\t'; i--) {
        line[i] = 0;
    }
}

// compare hash of file against recorded hash
// returns -1 if no hash found, -2 if file not found
// 0 if hashsum equal and positive if strcmp fails
int compare(char *dir, char *line) {
    regmatch_t pmatch[1];
    char hex[41];
    hex[40] = 0;

    // find hexstring and copy to hex, return -1 if no matches
    int match = regexec(&reghex, line, 1, pmatch, 0);
    if (match) return -1;
    strncpy(hex, line + pmatch[0].rm_so, 40);

    // trim line to find file
    trim(line, pmatch[0].rm_so);

    // copy to path and concat
    char path[PATH_MAX];
    strcpy(path, line);
    concat_path(dir, path);
    if (!check_exists(path, 1)) return -2;

    fprintf(stderr, "Checking %s...", get_relative_path(dir, path));

    // construct file_struct to give to hash function
    struct stat s;
    stat(path, &s);

    char *hash_value = hash(path, s.st_size);
    int result = strcmp(hash_value, hex);
    fprintf(stderr, "OK\n");

    // free malloc-ed variables
    free(hash_value);
    return !!result;
}

// frees regex allocs
void free_regex() {
    regfree(&reghex);
}
