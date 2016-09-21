/* tthreads.h */

#ifndef tthreads_included
#define tthreads_included

#include "setjmp.h"

typedef struct Thread_s {	/* a Thread */
  void (*initFn)(void *);	/* address of fn to call */
  void * initArg;		/* argument to initFn */
  jmp_buf context;
  int tid;			/* thread id */
  struct Thread_s *next;	/* pointer to another thread, for readyq */
  int enqueued;			/* used as bool, for checking */
} Thread;


/* Thread Queue (singly linked list) */
typedef struct {		
  Thread *head, *tail;
  int count;
} ThreadQ;
void initThreadQ(ThreadQ *tq);	
extern void enqueueThread(ThreadQ *tq, Thread *t);
extern Thread *dequeueThread(ThreadQ *tq);

extern ThreadQ readyQ;		/* ready queue (fcfs) */

/* 
 * Initialize thread system, creating <n> threads.
 * All threads put on "freshQ" awaiting work.
 */
extern void makeThreads(int n);	

/*
 * Assign some work to a thread (call to (*initFn)(initArg)).
 * Thread is placed on readyQ. (and thus ready)
 * Thread terminates when initFn returns.
 */
extern int createThread(void (*initFn)(void *), void *initArg) ;

/* Start running threads.
 * At least one thread must already be scheduled!
 * Returns when all threads have terminated.
 */
extern void startThreads();

extern int getTid();		/* get thread id */
extern void yield();		/* volunteer for preemption */


/*
 * Run another ready thread
 * If resched != 0:
 *   Enqueue current thread on queue <resched>
 */
extern void contextSwitch(ThreadQ *resched); 

#endif /* tthreads_included */

