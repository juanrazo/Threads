#include "tthreads.h"
/** \file mutex.h
 *  \brief A simple mutex
 */


/**  Mutex structure that contains the number of 
 *   instances available and a queue of threads
 *   that are blocked on this Mutex 
 */
typedef struct {
  int numAvailable;
  ThreadQ waitingThreads;
} Mutex;


/** This function initializes a mutex.
 *
 *  \param m The mutex to initialize
 *  \param available The number of instances that are available
 */
void mutexInit(Mutex *m, int available);

/** Take out a lock on a mutex.
 *
 *  This function attempts to take a lock out on Mutex m.
 *  If the lock isn't available the thread will block.
 *
 *  \param m The mutex to take a lock on
 *  \sa mutexUnlock()
 */
void mutexLock(Mutex *m);

/** Release a lock on a mutex.
 *
 *  This function will release a lock on a mutex. If 
 *  any threads are waiting on the mutex the first
 *  available thread is scheduled.
 *
 *  \param m The mutex to release
 *  \sa mutexLock()
 */
void mutexUnlock(Mutex *m);


     
