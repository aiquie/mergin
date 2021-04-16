#include "unifile.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buff[4096];
int const FDCOUNT = 64;

size_t remove_fd(int fd, int * fds, size_t fdc) {
    int res = fdc;
    while(fdc && *fds != fd) { ++fds; --fdc; }
    if(fdc) { memmove(fds, fds + 1, --fdc * sizeof(fd)); --res; }
    return res;
}

int main(int argc, char * const argv[]) {
    int opt;
    while((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h':
        default:
            fprintf(stderr, "Usage: %s [FILE]...\n", argv[0]);
            fprintf(stderr, "Concatenate FILE(s) to standard output by lines.\n");
            fprintf(stderr, "With no FILE, or when FILE is -, read standard input.\n");
            return EXIT_FAILURE;
        }
    }

    size_t fdc = 0;
    int fds[FDCOUNT];
    for(; optind < argc; ++optind) {
        if(fdc == FDCOUNT) {
            fprintf(stderr, "Error: large number of files defined.\n");
            return EXIT_FAILURE;
        }
        if(!strcmp(argv[optind], "-")) fds[fdc++] = STDIN_FILENO;
        else if((fds[fdc++] = open(argv[optind], O_RDWR)) == -1) {
            fprintf(stderr, "Error: %s does not open.\n", argv[optind]);
            return errno;
        }
    }
    if(!fdc) fds[fdc++] = STDIN_FILENO;

    int fd;
    while((fd = get_ready(fds, fdc)) != -1) {
        int read_size = 0;
        while((read_size = read(fd, buff, sizeof(buff))) > 0) {
            if(safe_write(STDOUT_FILENO, buff, read_size) == -1) return errno;
            if(buff[read_size - 1] == '\n') break;
        }
        if(read_size == -1) return errno;
        if(read_size ==  0) fdc = remove_fd(fd, fds, fdc);
    }

    return EXIT_SUCCESS;
}
