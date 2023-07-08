#ifndef datatypes_h
#define datatypes_h

typedef struct {
    char *name;
    size_t size;
    int checked;
} file_struct_t;

typedef struct dir {
    char *name;
    struct dir **folder;
    int contents;
} dir_t;
#endif
