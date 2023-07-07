char **files;
int file_count;

int loop_files(char *dir);
FILE *open_file(char *dir, char *src, char *opt);
char *get_line(FILE *f);
int check_exists(char *path, int file);
