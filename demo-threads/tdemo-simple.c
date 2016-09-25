#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"

/** struct that is passed to workers */
typedef struct {
  int workerNum;                        /**< worker number */
} TWork;

/** Simple worker that prints worker number. Note: 
 *  worker number doesn't have to be same as thread 
 *  ID.
 *
 *  \param w Work struct for this worker
 */
void worker(TWork *w) 
{
  int workerNum = w->workerNum;         /**< store worker number locally */
  printf("worker %d before yield\n", workerNum);
  yield();
  printf("worker %d after yield\n", workerNum);
  return;
}

int main()
{
  int numThreads = 3,                   /**< total number of threads */
      workerNum,                        /**< Worker iterator */
      i;                                /**< loop iterator */

  TWork *work;                          /**< Worker struct for current worker */

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
  
  
