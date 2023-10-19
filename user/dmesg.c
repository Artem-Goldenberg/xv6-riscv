#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    int p[2];
    pipe(p);
    char a = 'A';
    write(p[1], &a, 1);
    read(p[0], &a, 1);
    close(p[0]);
    close(p[1]);
    if (a != 'A') {
        printf("Somethings wrong ...\n");
        exit(1);
    }
    dmesg();
    exit(0);
}
