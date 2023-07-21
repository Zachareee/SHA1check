#ifndef datatypes_h
#define datatypes_h
typedef struct dir {
    char *name;
    struct dir **folder;
    int num;
} dir_t;

struct linux_dirent {
    long           d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[];
};
#endif
