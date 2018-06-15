#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "assrt.h"

int fd;

void lseek_inject (int argc) {
    if (argc == 2)
        assrt(lseek(fd, 23, SEEK_SET) >= 0, "lseek_failed\n");
}

int main (int argc, char **argv) {
    char out[6];
    int siz;
    fd = open("/dev/hello_stack", O_RDWR);

    write(fd, "dupa", 4);
    lseek_inject(argc);
    write(fd, "dooopa", 6);

    lseek_inject(argc);
    //printf("1\n");
    siz = read(fd, out, 4);
    assrt_eq(siz, 4, "first read");
    lseek_inject(argc);
    assrt_ncmp("oopa", out, 4, "first read");
    //printf("%d: (%.*s)\n", siz, siz, out);

    siz = read(fd, out, 6);
    assrt_eq(siz, 6, "second read");
    assrt_ncmp("dupado", out, 6, "second read");
    //printf("%d: (%.*s)\n", siz, siz, out);

    close(fd);
    return 0;
}

