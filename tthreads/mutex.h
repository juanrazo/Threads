#include "tthreads.h"

typedef struct {
  int numAvailable;
  ThreadQ waitingThreads;
} Mutex;

void mutexInit(Mutex *m, int available);
void mutexLock(Mutex *m);
void mutexUnlock(Mutex *m);


     
