#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINELEN 128

regex_t regex;

void get_line(FILE *f) {
    char *line = malloc(128 * sizeof(char));
    fgets(line, LINELEN, f);
    printf("%s: %s", line[strlen(line)-1] == '\n' ? "OK" : "BAD", line);
}

//FILE *find_file(char *dir) {
//    return NULL;
//}
//
//int compare_all(char *dir, char *src) {
//    return 0;
//}
