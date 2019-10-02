#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>

void usage(char *name) {
    fprintf(stderr, "\n\
     _         _\n\
  __| |_  ____| |\n\
 / _` \\ \\/ / _` |\n\
| (_| |>  < (_| |\n\
 \\__,_/_/\\_\\__,_|\n\
                  \n");
    fprintf(stderr, "Usage: %s directry\n", name);
}

int printdirent(struct dirent *dest_dir_entry) {
    do {
        struct dirent *d = dest_dir_entry;
        printf("d_ino       = %16lx\n", d->d_ino);
        printf("d_off       = %16lx\n", d->d_off);
        printf("d_reclen    = %16x\n", d->d_reclen);
        printf("d_type      = %16x\n", d->d_type);
        printf("d_name      = %s\n\n", d->d_name);

        dest_dir_entry = (void*)dest_dir_entry + dest_dir_entry->d_reclen;
    }while (dest_dir_entry->d_reclen  != 0);
    return 0;
}

char opthandler(char *opt) {
    if (strncmp(opt, "-", 1))
        return 1;
    opt++;
    return opt;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }

    DIR *dirp = opendir(argv[2]);
    if (dirp == NULL) {
        perror(argv[2]);
        exit(1);
    }

    int dest_fd = dirfd(dirp);
    struct dirent *dest_dir_entry = readdir(dirp);
    if (dest_dir_entry == NULL) {
        perror("readdir");
        exit(1);
    }

    if (!strncmp(argv[1], "-i", 2))
        printdirent(dest_dir_entry);
    else
        return 1;

    return 0;
}
