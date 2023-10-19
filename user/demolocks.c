#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: demolocks something\n");
        exit(1);
    }
    int iolock = createlock();
    char c;
    int pipe1[2], pipe2[2];
    pipe(pipe1); pipe(pipe2);
    if (fork()) {
        close(pipe2[1]);
        for (char *c = argv[1]; *c; ++c) write(pipe1[1], c, 1);
        close(pipe1[1]);
        acquirelock(iolock);
        while (read(pipe2[0], &c, 1))
            printf("%d: recieved %c\n", getpid(), c);
        releaselock(iolock);
        wait(0);
    } else {
        close(pipe1[1]);
        acquirelock(iolock);
        while (read(pipe1[0], &c, 1)) {
            printf("%d: recieved %c\n", getpid(), c);
            write(pipe2[1], &c, 1);
        }
        releaselock(iolock);
        close(pipe2[1]);
    }
    exit(0);
}
