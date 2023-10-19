#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "sleeplock.h"

#define NLOCKS 10
struct spinlock metalock;
struct {
    struct sleeplock lock;
    int isUsed;
} locks[NLOCKS];

void initlocks(void) {
    initlock(&metalock, "all locks");
    acquire(&metalock);
    for (int i = 0; i < NLOCKS; ++i) {
        initsleeplock(&locks[i].lock, "a lock");
        if (locks[i].isUsed != 0) printf("WRONG: %d\n", locks[i].isUsed);
    }
    release(&metalock);
}

/// - Returns: id of a new lock or -1 if no locks available
int createlock(void) {
    acquire(&metalock);
    for (int i = 0; i < NLOCKS; ++i) if (!locks[i].isUsed) {
        locks[i].isUsed = 1;
        release(&metalock);
        return i;
    }
    release(&metalock);
    return -1;
}

void acquirelock(int i) {
    acquiresleep(&locks[i].lock);
}

void releaselock(int i) {
    releasesleep(&locks[i].lock);
}

void deletelock(int i) {
    acquire(&metalock);
    locks[i].isUsed = 0;
    release(&metalock);
}
