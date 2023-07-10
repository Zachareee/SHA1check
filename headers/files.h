#include "datatypes.h"

void loop_files(char *dir, char *src, char *dst,
        dir_t *pass, dir_t *fail, dir_t *extra, unsigned int *count);
char *get_line(FILE *f);
int check_exists(char *path, int file);
void write_to_file(FILE *f, char *format, char *line);
void free_files();
