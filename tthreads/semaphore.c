#include "semaphore.h"

/*
  This code comes from the Mutex provided for the
  first part of the lab plus the algorithm from the
  book in section 2.3.5 which is just increasing or
  decreasing the numAvailable variable to make a
  counting semaphore.
*/
void semaphoreInit(Semaphore *s, int available){
  initThreadQ(&s->waitingThreads);
  s->numAvailable = available;
}

void down(Semaphore *s){
  yield();
  while(s->numAvailable==0){
    contextSwitch(&s->waitingThreads);
  }
  s->numAvailable--;
  yield();
}

void up(Semaphore *s){
  yield();
  s->numAvailable++;
  if(s->waitingThreads.count != 0)
    enqueueThread(&readyQ, dequeueThread(&s->waitingThreads));
  yield();
}

