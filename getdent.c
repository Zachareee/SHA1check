#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>

#define BUF_SIZE 1024
#include "datatypes.h"
#include "dir.h"
#include "paths.h"

dir_t *pass, *fail, *extras;
char *dir, *src, *dst;
unsigned int *count;

// recursive function to get directories
int getdent(char *name) {
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;

    fd = open(name, O_RDONLY | O_DIRECTORY);
    if (fd == -1) goto error;

    nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
    while (nread != 0) {
        if (nread == -1) goto error;

        for (bpos = 0; bpos < nread; bpos += d->d_reclen) {
            d = (struct linux_dirent *) (buf + bpos);
            if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")) continue;

            d_type = *(buf + bpos + d->d_reclen - 1);
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", name, (char *)d->d_name);

            if(d_type == DT_DIR) {
                getdent(path);
            } else {
                if (!strcmp(path, src) || !strcmp(path, dst)) continue;
                if (path_exists(pass, get_relative_path(dir, path))
                        || path_exists(fail, get_relative_path(dir, path))) continue;

                add_path_to_dir(get_relative_path(dir, path) , extras);
                (*count)++;
            }
        }

        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
    }

    return 0;

error:
    printf("Something went wrong while reading the directory\n");
    exit(-4);
}


// iterate through directories, path is absolute path
void file_iterator(char *dirl, char *srcl, char *dstl,
        dir_t *passl, dir_t *faill, dir_t *extral, unsigned int *countl) {
    pass = passl;
    fail = faill;
    extras = extral;
    dir = dirl;
    src = srcl;
    dst = dstl;
    count = countl;

    getdent(dir);
}
