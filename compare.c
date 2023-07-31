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

regex_t reghex1;
regex_t reghex2;

// init regex sequences
void regex_init() {
    int regh1 = regcomp(&reghex1, "[0-9a-fA-F]{40}", REG_EXTENDED);
    int regh2 = regcomp(&reghex2, "[0-9a-fA-F]{10}\\*\\*\\*[0-9a-fA-F]{10}", REG_EXTENDED);
    if (regh1 | regh2) {
        printf("Something went wrong while initialising pattern matcher\n");
        exit(-2);
    }
}

// if full hash given, return 1
// if partial hash given in the form of reghex2, returns 2
// if unknown, return 0
int get_hash_ver(char *line) {
    if (!regexec(&reghex1, line, 0, NULL, 0)) return 1;
    if (!regexec(&reghex2, line, 0, NULL, 0)) return 2;
    return 0;
}

// trim trailing whitespaces in the line
void trim(char *line, int offset) {
    for (int i = offset - 1; line[i] == ' ' || line[i] == '\t'; i--) {
        line[i] = 0;
    }
}

// obfuscates hash
void obf_hash(char *line, char *result) {
    strncpy(result, line, 10);
    strncpy(result + 10, "***", 3);
    strncpy(result + 13, line + 30, 10);
}

// compare hash of file against recorded hash
// returns -1 if no hash found, -2 if file not found
// 0 if hashsum equal and positive if strcmp fails
int compare(char *dir, char *line, int ver) {
    long result;
    char *hash_value;
    char hex[41] = {0};
    regmatch_t pmatch[1];
    int limit;
    regex_t *reg;

    if (ver == 1) {
        limit = 40;
        reg = &reghex1;
    } else {
        limit = 23;
        reg = &reghex2;
    }

    // find hexstring and copy to hex, return -1 if no matches
    int match = regexec(reg, line, 1, pmatch, 0);
    if (match) return -1;
    strncpy(hex, line + pmatch[0].rm_so, limit);

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

    hash_value = hash(path, s.st_size);

    if (ver == 1) {
        result = (long) strcmp(hash_value, hex);
    } else {
        long size;
        sscanf(line + pmatch[0].rm_eo, "%ld", &size);

        char temp[24];
        obf_hash(hash_value, temp);
        result = (long) strcmp(temp, hex);
        result |= s.st_size - size;
    }

    fprintf(stderr, "OK\n");

    // free malloc-ed variables
    free(hash_value);
    return !!result;
}

// frees regex allocs
void free_regex() {
    regfree(&reghex1);
    regfree(&reghex2);
}
