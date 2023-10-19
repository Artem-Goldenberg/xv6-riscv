#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"

#define NPAGES 2
#define DBUFSZ NPAGES * PGSIZE

struct {
    struct spinlock lock;
    char data[DBUFSZ];
    uint64 top;
} dbuf;

void initbuf(void) {
    initlock(&dbuf.lock, "msg buffer");
}

int entrysize(const char* str) {
    int n = 1; // count trailing \0
    while(*str++)++n;
    return n + sizeof ticks; // account for timestamp
}

void pr_msg(const char *msg) {
    int size = entrysize(msg);
    acquire(&dbuf.lock);
    if (dbuf.top + size <= DBUFSZ) {
        acquire(&tickslock);
        memmove(dbuf.data + dbuf.top, &ticks, sizeof ticks);
        release(&tickslock);

        memmove(dbuf.data + dbuf.top + sizeof ticks, msg, size - sizeof ticks);
        dbuf.top += size;
    }
    release(&dbuf.lock);
}

void dmesg(void) {
    acquire(&dbuf.lock);
    for (char *ptr = dbuf.data; ptr - dbuf.data < dbuf.top; ++ptr) {
        printf("[%d] ", *(uint*)ptr);
        ptr += sizeof(uint);
        while (*ptr) consputc(*ptr++);
        printf("\n");
    }
    release(&dbuf.lock);
}
