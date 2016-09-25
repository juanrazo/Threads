/** \file mutexTest.c
 *  \brief An example of how to use the mutex to
 *  to control access to critical regions.
 */
#include "tthreads.h"
#include "stdio.h"
#include "mutex.h"

Mutex m;                                /**< Single instance of resource for all workers */

/** Worker thread, attempts to take lock and 
 *  prints what part of critical section it's in.
 */
void worker(void *ignored) 
{
  int i;
  for (i = 0; i < 2; i++) {
    fprintf(stderr, "==thread %d outside critical section!\n", getTid());
    yield();
    mutexLock(&m);
    fprintf(stderr, ".thread %d in critical section!\n", getTid());
    yield();			/* mix things up */
    fprintf(stderr, "..thread %d still in critical section!\n", getTid());
    yield();			/* mix things up */
    fprintf(stderr, "...thread %d about to leave critical section!\n", getTid());
    mutexUnlock(&m);
    fprintf(stderr, "==thread %d outside critical section!\n", getTid());
    yield();
  } 
}

/** Inits mutex, creates 2 worker threads, and starts threading system */
int main()
{
  mutexInit(&m, 1);
  makeThreads(2);
  createThread(worker, 0);
  createThread(worker, 0);
  startThreads();
  return 0;
}
  
  
