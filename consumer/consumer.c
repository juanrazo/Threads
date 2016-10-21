#include "stdio.h"
#include "stdlib.h"
#include "tthreads.h"
#include "semaphore.h"
#include "mutex.h"

#define MAX 10
Semaphore empty;
Semaphore full;

int flag = 0;
typedef struct{
  int queue[10];
  int front;
  int rear;
  int count;
  Mutex lock;
}Buffer;
Buffer buff;

void initSemaphores(){
  semaphoreInit(&empty, MAX);
  semaphoreInit(&full, 0);
}

void initBuff(){
  Buffer *b = &buff;
  
  b->front = 0;
  b->rear = -1;
  b->count = 0;
  mutexInit(&b->lock, 1);
}

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
void producer(){
  int item = 0;
  
  while(item <100){
    down(&empty);
    mutexLock(&(&buff)->lock);
    enqueue(&buff, item);
    mutexUnlock(&(&buff)->lock);
    up(&full);
    printf("Pushed %d\n", item);
    item++;
  }
  flag++;
}

void consumer(){
  int total = 0;
  int consume;
  int consumeFlag=1;
  while(consumeFlag){
    down(&full);
    mutexLock(&(&buff)->lock);
    consume = dequeue(&buff);
    printf("Poped %d\n", consume);
    mutexUnlock(&(&buff)->lock);
    up(&empty);
    total = total + consume;
    printf("Total count %d\n", total);
    if(flag ==4 && isBufferEmpty())
      consumeFlag=0;
  }
}

int main(){
  int numThreads = 5;
  initSemaphores();
  initBuff();

  makeThreads(numThreads);

  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))producer, &buff);
  createThread((void (*)(void *))consumer, &buff);

  startThreads();
  printf("Threads Finished \n");
  return 1;
}
