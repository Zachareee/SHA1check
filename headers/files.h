#include "datatypes.h"

void file_iterator(char *dir, char *src, char *dst,
        dir_t *pass, dir_t *fail, dir_t *extra, unsigned int *count);
char *get_line(FILE *f);
int check_exists(char *path, int file);
void write_to_file(FILE *f, char *format, char *line);
void free_files();
void write_dir_and_filecount(FILE *f, dir_t *dir, char *string);