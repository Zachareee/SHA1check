#ifndef datatypes_h
#define datatypes_h
typedef struct dir {
    char *name;
    struct dir **folder;
    int num;
} dir_t;
#endif
