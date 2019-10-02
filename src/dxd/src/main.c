#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>

void usage(const char *name) {
    fprintf(stderr, "\n\
     _         _\n\
  __| |_  ____| |\n\
 / _` \\ \\/ / _` |\n\
| (_| |>  < (_| |\n\
 \\__,_/_/\\_\\__,_|\n\
                  \n\
Usage: %s -ina directry\n\
<options>\n\
    -i             :   inode of dest directory(or normal file).\n\
    -n             :   like \"ls -a\".\n\
    -a  <default>  :   show dest directory's dirent mamber.\n", name);
}

static struct dirent *direntByPath(const char *path) {
    DIR *dirp = opendir(path);
    if (dirp == NULL)
        return NULL;

    struct dirent *dest_dir_entry = malloc(sizeof(struct dirent));
    dest_dir_entry = readdir(dirp);
    if (dest_dir_entry == NULL)
        return NULL;

    return dest_dir_entry; 
}

void printdirent_inode(const char *path) {
    ino_t ino;
    struct dirent *dest = direntByPath(path);
    if (dest == NULL) {
        struct stat *buf;
        lstat(path, buf);
        ino = buf->st_ino;
    } else
        ino = dest->d_ino;

    printf("%s's inode is %016lx\n", path, ino);
}

void printdirent_name(const char *path) {
    struct dirent *dest = direntByPath(path);
    if (dest == NULL) {
        perror(path);
        exit(1);
    }

    do {
        printf("%s ", dest->d_name);
        dest = (void*)dest + dest->d_reclen;
    }while (dest->d_reclen != 0);
    printf("\n");
}

void printdirent_all(const char *path) {
    struct dirent *dest_dir_entry = direntByPath(path);
    if (dest_dir_entry == NULL) {
        perror(path);
        exit(1);
    }

    do {
        struct dirent *d = dest_dir_entry;
        printf("d_ino       = %16lx\n", d->d_ino);
        printf("d_off       = %16lx\n", d->d_off);
        printf("d_reclen    = %16x\n", d->d_reclen);
        printf("d_type      = %16x\n", d->d_type);
        printf("d_name      = %s\n\n", d->d_name);

        dest_dir_entry = (void*)dest_dir_entry + dest_dir_entry->d_reclen;
    }while (dest_dir_entry->d_reclen  != 0);
}

int opthandler(char *opt) {
    if (strncmp(opt, "-", 1) != 0)
        return 1;
    opt++;
    return *opt;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }

    const char* dirpath = (argc == 3)?argv[2]:argv[1];
    void (*func)(const char*);

    switch (opthandler(argv[1])) {
        case   1:
        case 'a':
            func = printdirent_all;
            break;
        case 'i':
            func = printdirent_inode;
            break;
        case 'n':
            func = printdirent_name;
            break;
        default:
            func = usage;
            break;
    }

    func(dirpath);

    return 0;
}
