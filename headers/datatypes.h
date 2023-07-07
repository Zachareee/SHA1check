#ifndef datatypes_h
#define datatypes_h

typedef struct {
    char *name;
    size_t size;
    int checked;
} file_struct;

typedef struct dir {
    char *name;
    struct dir *folder;
    int contents;
} dir;
#endif
