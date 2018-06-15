#ifndef __ASSRT_H__
#define __ASSRT_H__

#include <stdarg.h>
#include <string.h>

void assrt(int out, const char *message, ...) {
    va_list arglist;
    va_start(arglist, message);
    if (out != 1) {
        printf("ASSERT_FAILED\n");
        vprintf(message, arglist);
    }
    va_end(arglist);
}

void assrt_eq(int a, int b, const char *msg) {
    assrt(a == b, "expected %d == %d in %s\n", a, b, msg);
}

void assrt_ncmp(const char *a, const char *b, int len, const char *msg) {
    assrt(strncmp(a, b, len) == 0, "expected ((%.*s)) == ((%.*s)) in %s\n", len, a, len, b, msg);
}

#endif
