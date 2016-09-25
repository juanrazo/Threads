#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"

typedef struct {
  int workerNum;
} TWork;

void worker(TWork *w) 
{
  int workerNum = w->workerNum;
  printf("worker %d before yield\n", workerNum);
  yield();
  printf("worker %d after yield\n", workerNum);
  return;
}


int main()
{
  int numThreads = 3, workerNum, i;

  TWork *work;

  for (i = 0; i < 2; i++) {
    makeThreads(numThreads);
    for (workerNum = 0; workerNum < numThreads; workerNum++) {
      work = (TWork *)malloc(sizeof(TWork));
      work->workerNum = workerNum;
      createThread((void(*)(void*))worker, work);
    }
    
    printf("about to start threads\n");
    startThreads();		/* start working! */
    printf("Threads Terminated\n");
  }
  return 0;
}
  
  
