#include "datatypes.h"

void add_path_to_dir(char *path, dir_t *dir);
void free_dir(dir_t *dir);
void write_dir_to_file(dir_t *dir, int level, FILE *f);
int path_exists(dir_t *dir, const char *path);
int count_files_in_dir(dir_t *dir);