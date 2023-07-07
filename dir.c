#include <stdlib.h>

#include "datatypes.h"

dir *create_dir() {
    return malloc(sizeof(dir));
}
