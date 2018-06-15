#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "assrt.h"

int main (int argc, char **argv) {
    if (argc < 3 || argc > 4) {
        printf ("usage: %s read_size expected_text\n", argv[0]);
        return -1;
    };
    int in_siz = atoi(argv[1]);
    char out[in_siz];
    int fd = open("/dev/hello_stack", O_RDWR);

    int siz = read(fd, out, in_siz);

    int exp_siz = in_siz;
    if (argc == 4) exp_siz = atoi(argv[3]);
    assrt_eq(exp_siz, siz, "read call");
    assrt_ncmp(argv[2], out, exp_siz, "read call");

    close(fd);
    return 0;
}

