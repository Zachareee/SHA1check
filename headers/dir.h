#include "datatypes.h"
void append_dir(dir_t *dir, char *name);
void add_path_to_dir(char *path, dir_t *dir);
void free_dir(dir_t *dir);
void print_dir(dir_t *dir, int level);
