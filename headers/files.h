char **files;
int file_count;

int loop_files(char *dir);
FILE *open_file(char *dir, char *src, char *opt);
void get_real_path(char **dir);
char *get_line(FILE *f);
