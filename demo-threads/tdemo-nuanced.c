#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"

/** struct that is passed to workers */
typedef struct {
  int workerNum;                        /**< worker number */
} TWork;

/** Worker thread that will either print it's worker
 *  number and thread ID several times, or will create
 *  another worker. Note: worker number doesn't have to
 *  be the same as thread ID.
 *
 *  \param w Work struct for this worker
 */
void worker(TWork *w) 
{
  int workerNum = w->workerNum;         /**< Store worker number locally */
  int i;
  if (workerNum) {		        /* start worker <workerNum>-1 */
    TWork *nextWork = (TWork *)malloc(sizeof(TWork));
    nextWork->workerNum = workerNum -1;
    createThread((void (*)(void *))worker, nextWork);
    yield();                            /* volunteer for preemption */
  }
  for (i=3*workerNum; i >= 0; i--) {    /* iterate 3*<workerNum> times */
    fprintf(stderr, "workerNum=%d, at iteration %d (tid=%d)\n", workerNum, i, getTid());
    yield();                            /* volunteer for preemption */
  }
  fprintf(stderr, "worker %d (tid=%d) terminating\n", workerNum, getTid());
  free(w);
  return;
}


int main()
{
  int numThreads = 3,                   /**< total number of threads */
      i;                                /**< Loop iterator */
  makeThreads(numThreads);           

  TWork *work = (TWork *)malloc(sizeof(TWork));  /**< Worker struct for current worker */
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
  
  
