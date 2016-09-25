# os-threads

Operating systems threads lab

## Introduction to thread system

The tthreads package is a non-preemptive user mode threading library.
This document describes
how it is intended to be used.  Its 
[API is also documented online](https://cdn.rawgit.com/robustUTEP/os-threads/master/html_doc/html/index.html)

Step 1: makeThreads initializes the thread system. It must be called
prior to any other functions from the tthreads library.

Step 2: createThread creates a new ready thread. It can be called anytime after
makeThreads. createThread takes two arguments, the address of the function that
the thread will call upon beginning execution and its parameter. A thread terminates
when that function returns.

Step 3: startThreads starts the threading system by scheduling one of the ready
threads. startThreads returns when either (1) all threads have terminated or (2)
the thread system has deadlocked. The thread system is deadlocked when there are
unterminated threads and no threads are ready.

*Notes*
1. A thread may call yield at any time. A call to yield changes the running (calling)
thread's state from running to ready.

## Introduction to the mutex library

The mutex library implements a blocking binary semaphore of type Mutex that controls
access to a singleton resource. The interface is described below.

1. mutexInit initializes the Mutex with its initial availability (either 0 if unavailable or
non-zero if available).

2. mutexLock requires access to the resource. If the resource is unavailable the thread
will block until the thread is granted access.

3. mutexUnlock releases the resource thereby making it available to be locked in the future.

## Programs built using tthreads library

* [demo-threads](demo-threads)
* [bank transfer programs (including first part of assignment)](bank)
