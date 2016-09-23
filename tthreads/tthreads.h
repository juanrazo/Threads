/** \file tthreads.h 
 *  \brief A basic threading system implementation. 
 *  This contains the basic pieces needed to implement 
 *  a threading system, including run queues
 *  and associated functions.
 */

#ifndef tthreads_included
#define tthreads_included

#include "setjmp.h"

/** A thread structure that contains the information needed to run a thread. */
typedef struct Thread_s {	/* a Thread */
  void (*initFn)(void *);   /**< Address of fn to call when thread is 1st run*/
  void * initArg;           /**< argument to initFn */
  jmp_buf context;          /**< threads current context */ 
  int tid;                  /**< thread id */
  struct Thread_s *next;    /**< pointer to another thread, for readyq */
  int enqueued;             /**< used as bool, for checking */
} Thread;


/** Thread Queue (singly linked list). */
typedef struct {		
  Thread *head;             /**< Pointer to the head of the thread queue */
  Thread *tail;             /**< Pointer to the tail of the thread queue */
  int count;                /**< Number of threads in this queue */
} ThreadQ;

extern ThreadQ readyQ;		/**< ready queue (fcfs) */

/** Initialize a thread queue.
 *
 * \param tq the thread queue to initialize
 * \sa enqueueThread() dequeueThread()
 */
void initThreadQ(ThreadQ *tq);

/** Enqueue a thread onto a thread queue. 
 *
 * \param t The thread to enqueue
 * \param tq The thread queue where the thread will be queued
 * \sa enqueueThread()
 */
extern void enqueueThread(ThreadQ *tq, Thread *t);

/** Dequeue a thread from a thread queue. 
 *
 * \param tq The thread queue where the thread will be dequeued
 * \return The thread that was dequeued
 * \sa enqueueThread()
 */
extern Thread *dequeueThread(ThreadQ *tq);

/** Initialize thread system, creating <n> threads.
 *  All threads put on "freshQ" awaiting work. New
 *  Threads need to be intialized using createThread()
 *
 *  \param n The number of threads to create
 *  \sa createThread()
 */
extern void makeThreads(int n);	

/** Assign some work to a thread (call to (*initFn)(initArg)).
 *  Thread is placed on readyQ. (and thus ready). Thread 
 *  terminates when initFn returns.
 *
 *  \param initFn Address of fn to call when thread is 1st run
 *  \param initArg argument to initFn
 */
extern int createThread(void (*initFn)(void *), void *initArg) ;

/** Start running threads.
 *  At least one thread must already be scheduled!
 *  Returns when all threads have terminated.
 */
extern void startThreads();

/** Returns the current thread's id
 *
 * \return Thread's id
 */
extern int getTid();

/** Allows a thread to volunteer for preemption
 */
extern void yield();

/** Run another ready thread
 *  If resched == 0: make this thread ready
 *  Otherwise:  Enqueue current thread on queue <resched>
 *
 *  \param resched The ready queue on which we are context switching
 */
extern void contextSwitch(ThreadQ *resched); 

#endif /* tthreads_included */

