#include <stdio.h>
#include <string.h>

typedef struct Vulnerable {
    char *ptr;
    char buffer[16];
} Vulnerable;

int main(int argc, char **argv) {
    Vulnerable v;
    v.ptr = NULL;
    printf("ptr: %p\n", v.ptr);

    if (argc > 1) {
        strcpy(v.buffer, argv[1]);
        printf("Input was: %s\n", v.buffer);
        printf("ptr: %p\n", v.ptr);
    }
    return 0;
}
