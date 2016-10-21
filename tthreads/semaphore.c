#include "semaphore.h"


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

