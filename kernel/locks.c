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

/** 
 Initialize all the locks **NOT REALLY** @a r @b r @c abc;

 - Warning none: something

 \c abracadabra
 @e something @n
 @n something @n more @n and even more
 @throw \c Runtime \c Exception: avoid that
 @thread main
 @warning be cautious
 @warning something
 @note nothign is wrong

 @author me
 @code int i = 0; i < 0; i++

 */
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


/** 
 Some

 # Some stuff
 Here it is
 ## More code
 ```
 code
 ```
 ### And What is this?
 This is just a markdown

 [Smart Link](https://www.doxygen.nl/manual/docblocks.html)

 - Experiment: something interesiont \
 And even more

 - Parameter i: `ID` of the lock to acquire
 - Invariant: something and even more something
 - Important: Do not cry
 - Note: "There is nothing either good or bad, but thinking makes it so." \
 \
 Hamlet in (*Hamlet, 2.2*) by William Shakespeare

 ![Xcode icon](http://devimages.apple.com.edgekey.net/assets/elements/icons/128x128/xcode.png)

 */
//![some](http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4)
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
