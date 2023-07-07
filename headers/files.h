extern char **files;
extern int file_count;

int loop_files(char *dir);
char *get_line(FILE *f);
int check_exists(char *path, int file);
void mark_file(char *dir, char *filename);
