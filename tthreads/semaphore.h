#include "tthreads.h"
/*
  This comes from the mutex.h to implement
  a counting semaphore.
*/
typedef struct{
  ThreadQ waitingThreads;
  int numAvailable;
}Semaphore;

void semaphoreInit(Semaphore *s, int available);

void up(Semaphore *s);

void down(Semaphore *s);
