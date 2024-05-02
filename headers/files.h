#include "datatypes.h"
#include <stdbool.h>

void file_iterator(char *dir, char *src, char *dst,
        dir_t *pass, dir_t *fail, dir_t *extra, unsigned int *count);
char *get_line(FILE *f);
int check_exists(char *path, int file);
void write_to_file(FILE *f, char *format, char *line);
int write_file_count(dir_t *directory, char *message, FILE *checkfile, bool write_dir);
bool is_a_file(char *path);
bool is_a_directory(char *path);
