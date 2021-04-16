#include "unifile.h"

#include <errno.h>
#include <stdlib.h>

int get_ready(int * fds, size_t fdc) {
    fd_set rfds;
    FD_ZERO(&rfds);
    size_t i = 0;

    int maxfd = -1;
    for(i = 0; i < fdc; ++i) {
        FD_SET(fds[i], &rfds);
        maxfd = fds[i] > maxfd ? fds[i] : maxfd;
    }

    if(maxfd == -1) return -1;
    if(select(maxfd + 1, &rfds, NULL, NULL, NULL) == -1) return -1;
    for(i = 0; i < fdc; ++i) {
        if(FD_ISSET(fds[i], &rfds)) return fds[i];
    }
    return -1;
}

ssize_t safe_read(int fd, void * buff, size_t size) {
    ssize_t res = 0;
    while(size) {
        ssize_t ret = read(fd, buff, size);
        if(ret == -1 && errno == EAGAIN) ret = 0;
        if(ret == -1) return -1;
        if(ret ==  0) break;
        res  += ret;
        buff += ret;
        size -= ret;
    }
    return res;
}

ssize_t safe_write(int fd, const void * buff, size_t size) {
    ssize_t res = size;
    while(size) {
        ssize_t ret = write(fd, buff, size);
        if(ret == -1 && errno == EAGAIN) ret = 0;
        if(ret == -1) return -1;
        buff += ret;
        size -= ret;
    }
    return res;
}
