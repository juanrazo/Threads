#include "stdio.h"
#include "stdlib.h"
#include "tthreads.h"
#include "semaphore.h"
#include "mutex.h"

#define MAX 10
#define THREADS 5

Semaphore empty;
Semaphore full;

//Flag to keep track of Producers
int flag = 0;

//Buffer with size 10
typedef struct{
  int queue[MAX];
  int front;
  int rear;
  int count;
  Mutex lock;
}Buffer;
Buffer buff;

// Initialize the semaphores
void initSemaphores(){
  semaphoreInit(&empty, MAX);
  semaphoreInit(&full, 0);
}

// Initialize the buffer
void initBuff(){
  Buffer *b = &buff;
  
  b->front = 0;
  b->rear = -1;
  b->count = 0;
  mutexInit(&b->lock, 1);
}

/* Functions for Buffer queue
   The idea and some code for the queue
   comes from tutorialspoint.com
   tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm
*/
int isFull(Buffer *buffer){
  return (buffer->count == MAX) ? 1 : 0;
}

int isEmpty(Buffer *buffer){
  return (buffer->count==0) ? 1 : 0;
}

int size(Buffer *buffer){
  return buffer->count;
}

void enqueue(Buffer *buffer, int num){
  if(!isFull(buffer)){
    if(buffer->rear == (MAX-1)){
      buffer->rear = -1;
    }
    buffer->queue[++buffer->rear] = num;
    buffer->count++;
  } 
}

int dequeue(Buffer *buffer){
  int data = buffer->queue[buffer->front++];

  if(buffer->front == MAX)
    buffer->front = 0;

  buffer->count--;
  return data;
}

int isBufferEmpty(){
  int num;
  mutexLock(&(&buff)->lock);
  num = isEmpty(&buff);
  mutexUnlock(&(&buff)->lock);
  return num;
}

/* 
   The producer code comes from the OS book, section
   2.3.5 Semaphores. The only Difference is the 
   print, incrase item and the flag when its done
   producing. 
*/
void producer(){
  int item = 0;
  
  while(item <100){

    down(&empty);
    mutexLock(&(&buff)->lock);

    enqueue(&buff, item);

    mutexUnlock(&(&buff)->lock);
    up(&full);

    //printf("Produced %d\n", item);
    item++;
  }
  flag++;
}
/* 
   The consumer is also from the OS book on section
   2.3.5. The total variable is to keep track of the
   produced integers. The consume is just to save the
   dequeue value and the consumeFlag is to check if
   all producers are done. The consumer will stop
   consuming when the all producers are done and the
   buffer is empty.
*/
void consumer(){
  int total = 0;
  int consume;
  int consumeFlag=1;
  while(consumeFlag){

    down(&full);
    mutexLock(&(&buff)->lock);

    consume = dequeue(&buff);
    //printf("Consumed %d\n", consume);

    mutexUnlock(&(&buff)->lock);
    up(&empty);
    total = total + consume;
    //printf("Total count %d\n", total);

    if((flag == (THREADS-1)) && isBufferEmpty())
      consumeFlag=0;
  }
  printf("Done consuming %d\n", total);
}

int main(){
 
  initSemaphores();
  initBuff();

  makeThreads(THREADS);

  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))consumer, &buff);

  startThreads();
  printf("Threads Finished \n");

  return 1;
}
