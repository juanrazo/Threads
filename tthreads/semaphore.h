#include "tthreads.h"

typedef struct{
  ThreadQ waitingThreads;
  int numAvailable;
}Semaphore;

void semaphoreInit(Semaphore *s, int available);

void up(Semaphore *s);

void down(Semaphore *s);
