#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"

typedef struct {
  int workerNum;
} TWork;

void worker(TWork *w) 
{
  int workerNum = w->workerNum;
  int i;
  if (workerNum) {		/* start worker <workerNum>-1 */
    TWork *nextWork = (TWork *)malloc(sizeof(TWork));
    nextWork->workerNum = workerNum -1;
    createThread((void (*)(void *))worker, nextWork);
    yield();			/* vollunteer for preemption */
  }
  for (i=3*workerNum; i >= 0; i--) { /* iterate 3*<workerNum> times */
    fprintf(stderr, "workerNum=%d, at iteration %d (tid=%d)\n", workerNum, i, getTid());
    yield();			/* vollunteer for preemption */
  }
  fprintf(stderr, "worker %d (tid=%d) terminating\n", workerNum, getTid());
  free(w);
  return;
}


int main()
{
  int numThreads = 3, i;
  makeThreads(numThreads);

  TWork *work = (TWork *)malloc(sizeof(TWork));
  work->workerNum = numThreads-1;
  createThread((void(*)(void*))worker, work);
  startThreads();		/* start working! */
  printf("Threads Terminated\n");

  makeThreads(numThreads);
  work = (TWork *)malloc(sizeof(TWork));
  work->workerNum = numThreads-1;
  createThread((void (*)(void*))worker, work);
  startThreads();		/* start working! */
  printf("Threads Terminated\n");


  return 0;
}
  
  
