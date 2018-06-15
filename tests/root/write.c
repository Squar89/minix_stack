#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void assrt(int out, const char *message) {
    if (out != 1) printf("ASSERT_FAILED: %s\n", message);
}

int main (int argc, char **argv) {
    if (argc != 2) {
        printf ("usage: %s text_to_write\n", argv[0]);
        return -1;
    };
    int siz;
    int fd = open("/dev/hello_stack", O_RDWR);

    write(fd, argv[1], strlen(argv[1]));

    close(fd);
    return 0;
}

