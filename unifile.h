#ifndef UNIFILE_H
#define UNIFILE_H

#include <unistd.h>

int get_ready(int * fds, size_t fdc);

ssize_t safe_read( int fd, void       * buff, size_t size);
ssize_t safe_write(int fd, const void * buff, size_t size);

#endif //UNIFILE_H

