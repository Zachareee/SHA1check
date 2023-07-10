#include "datatypes.h"
extern file_struct_t *files;
extern int file_count;

void loop_files(char *dir);
char *get_line(FILE *f);
int check_exists(char *path, int file);
void mark_file(char *dir, char *filename);
void write_to_file(FILE *f, char *format, char *line);
void free_files();
