#include <stdlib.h>

#include "datatypes.h"

// creates directory and initialises variables
dir_t *create_dir(char *name) {
    dir_t *dir = malloc(sizeof(dir_t));
    dir->name = name;
    return dir;
}


