#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

// function prototypes
void execute(void (*somefunction)(void *p), void *p);
int pool_submit(void (*somefunction)(void *p), void *p);
void *worker(void *param);
void pool_init(int num_threads);
void pool_shutdown(void);

typedef struct 
{
    void (*function)(void *p);
    void *data;
} Task;

typedef struct Node
{
    Task task;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *head;
    Node *rear;
} Queue;

int enqueue(Queue* queue, Task t);
Node* dequeue(Queue* queue);
Queue* queue_init();
