#include "mutex.h"

void mutexInit(Mutex *m, int available) {
  initThreadQ(&m->waitingThreads);
  m->numAvailable = available;
}

void mutexLock(Mutex *m) {
  yield();			/* just to mix things up a bit! */
  while (m->numAvailable == 0) {
    contextSwitch(&m->waitingThreads); /* block */
  }
  m->numAvailable = 0;
  yield();			/* just to mix things up a bit! */
}

void mutexUnlock(Mutex *m) {
  yield();			/* just to mix things up a bit! */
  m->numAvailable = 1;
  if (m->waitingThreads.count != 0) /* make another thread ready */
    enqueueThread(&readyQ, dequeueThread(&m->waitingThreads));
  yield();			/* just to mix things up a bit! */
}


     
