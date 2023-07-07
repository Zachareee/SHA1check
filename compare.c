#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINELEN 12

regex_t regex;

char *get_line(FILE *f) {
    char buffer[LINELEN];
    char *line = malloc(LINELEN * sizeof(char));
    line[0] = '\0';

    while(fgets(buffer, LINELEN, f) != NULL) {
        printf("%s\n", line);
        strcat(line, buffer);
        if (line[strlen(line) - 1] == '\n') return line;
        line = realloc(line, (strlen(line) + LINELEN) * sizeof(char));
    }

    printf("This line does not end with a linebreak: %s", line);
    free(line);
    return NULL;
}

//FILE *find_file(char *dir) {
//    return NULL;
//}
//
//int compare_all(char *dir, char *src) {
//    return 0;
//}
