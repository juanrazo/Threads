#include "stdlib.h"
#include "stdio.h"
#include "assert2.h"
#include "tthreads.h"

Thread *threads,                     /**< A list of all the threads within the system */    
       *currentThread = (Thread *)0; /**< current thread, zero when running main */
static int numThreads = 0;           /**< Number of total threads in the system */

ThreadQ readyQ;                      /**< The queue of threads that are ready to run */
static ThreadQ freshQ;		         /**< queue of fresh (never run) threads */
static jmp_buf mainContext;	         /**< context of main thread */


void initThreadQ(ThreadQ *tq) {
  tq->head = tq->tail = (Thread*)0;
  tq->count = 0;
}

void enqueueThread(ThreadQ *tq, Thread *t) {
  tq->count++;
  assert2(!t->enqueued, "Attempted to enqueue an already already enqueued thread!");
  assert2(!t->next, "Attempted to enqueue thread with a next????");
  if (tq->head) {		/* queue not empty */
    assert2(tq->count > 1, "corrupt queue count!");
    tq->tail->next = t;
    tq->tail = t;
  } else {			/* queue was empty */
    assert2(tq->count == 1, "corrupt queue count!");
    tq->head = tq->tail = t;	/* insert singleton */
  }
  t->enqueued = 1;
}

Thread *dequeueThread(ThreadQ *tq) { /* return 0 if no thread enqueued */
  Thread *t = (Thread *)0;	/* thread to return */
  if (tq->head) {		/* if Q non empty */
    t = tq->head;		/* get thread */
    tq->head = t->next;		/* remove from queue */
  }
  if (t) {
    assert2(tq->count > 0, "corrupt queue count!");
    tq->count--;
    assert2(t->enqueued, "Thread dequeued twice without reqeueing????");
    t->enqueued = 0;
    t->next = (Thread *)0;	/* forget next (not on queue) */
  } else {
    assert2(tq->count == 0, "corrupt queue count!");
  }
  return t;
}
/*
  Yield cpu.
  Reschedule onto <tq> if not zero.
  System terminates if no thread is ready.
*/
void contextSwitch(ThreadQ *tq) {	
  assert2(currentThread, "Can't context switch away from main!");
  if (setjmp(currentThread->context)) /* save context, return 0 unless restore */
    return;
  if (tq) 			/* don't resched if tq=0 */
    enqueueThread(tq, currentThread);
  currentThread = dequeueThread(&readyQ); /* next thread to run */
  if (currentThread) {		
    longjmp(currentThread->context, 1);	/* restore other thread's context */
  } else {			/* no ready threads -- terminate */
    assert2(freshQ.count == numThreads, "No runnable threads and at least one is unterminated! (deadlock)");
    longjmp(mainContext, 1);	/* done, return to startThreads */
  }
}

/*
  Start executing ready threads.
  Returns when all threads have terminated.
*/
void startThreads()		
{
  assert2(!currentThread, "Threads already started!");
  if (!setjmp(mainContext)) {	/* start threads */
    currentThread = dequeueThread(&readyQ);
    assert2(currentThread, "No threads to run???");
    longjmp(currentThread->context, 1);
  } else { 			/* all threads terminated */
    free(threads);
    threads = currentThread = (Thread *)0;
    initThreadQ(&readyQ); initThreadQ(&freshQ);
  }
}


/* mutually recursive helper functions for makeThreads() */
static void _makeThreads2(int, char*, jmp_buf*); /* forward declaration */
static void _makeThreads1(int n, jmp_buf *initContext)	/* allocate 64k on stack */
{
  char stackSpace[(1<<16)];	/* reserve 64k on stack */
  _makeThreads2(n, stackSpace, initContext); /* ref to stackSpace (don't optimize away) */
}
void _makeThreads2(int n, char *ignored, jmp_buf *initContext) /* save context; 2nd arg is ignored */
{
  Thread *t = &threads[n];
  t->tid = n;		/* set tid  */
  enqueueThread(&freshQ, t);
  /* initial context */
  if (setjmp(t->context)) {	/* setjmp returns 0 unless target of longjmp */
    for (;;) {
      assert2(t->initFn, "thread not initialized");
      (t->initFn)(t->initArg); /* will continue here first time run */
      /* thread terminated! */
      assert2(currentThread, "TerminateThread called from outside thread context!");
      currentThread->initFn = (void *)0;	/* make ready for new work */
      contextSwitch(&freshQ); 	/* put back on queue of fresh threads */
    }
  }
  if (n > 0)			/* make other threads! */
    _makeThreads1(n-1, initContext);
  else
    longjmp(*initContext, 1);	/* "return" to makeThreads() via longjmp */
}

/*
  Create pool of <n> threads (only call once) 
  New threads are blocked until they are initialized.
  Initialize new threads using createThread()
*/
void makeThreads(int n) {		
  jmp_buf initContext;
  assert2(!threads, "makeThreads called multiple times!");
  numThreads = n;
  threads = (Thread *)calloc(sizeof(Thread), n);
  assert2(threads, "allocation of thread memory failed");
  if (!setjmp(initContext))
    _makeThreads1(n-1, &initContext);
}


/*
  Prepare a thread for execution.
  Returns tid
  Only call once per thread
*/
int createThread(void (*initFn)(void *), void *initArg) 
{
  Thread *t = dequeueThread(&freshQ);
  assert2(threads, "Threads not yet created (call makeThreads())");
  assert2(t, "No available threads.");
  int tid = t->tid;
  assert2(tid >= 0, "tid must not be negative!");
  assert2(tid < numThreads, "tid out of range!");
  assert2(!t->initFn, "Thread initialized more than once!");
  t->initFn = initFn; t->initArg = initArg;
  enqueueThread(&readyQ, t);
  return tid;
}

int getTid() {
  assert2(currentThread, "getTid can not be called outside of a thread!");
  return currentThread->tid;
}

void yield() 			/* voluntarily offer to preempt */
{
  if (readyQ.count)		/* if readyQ not empty */
    contextSwitch(&readyQ);	/* make self ready */
}

  
