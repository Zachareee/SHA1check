#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <files.h>
#include <hashing.h>
#include <paths.h>
#define LINELEN 12

regex_t reghex;
regex_t regws;

int comp_init() {
    int regh = regcomp(&reghex, "[0-9a-zA-Z]{40}", REG_EXTENDED);
    int regw = regcomp(&regws, "\\s", REG_EXTENDED);
    return regh + regw;
}

void trim(char *line, int offset) {
    for (int i = offset - 1; line[i] == ' '; i--) {
        line[i] = 0;
    }
}

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
    concat_path(dir, &line);

    struct stat s;
    stat(line, &s);
    file_struct f = {line, s.st_size};
    char *hash_value = hash(f);
    printf("%s\n%s\n", hash_value, hex);
    int result = strcmp(hash_value, hex);

    free(hash_value);
    free(line);
    return !!result;
}
