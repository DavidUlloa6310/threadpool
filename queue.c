#include "threadpool.h"

pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;

Queue* queue_init() {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q -> head = NULL;
    q -> rear = NULL;
    return q;
}
 
Node* dequeue(Queue* queue) 
{
    if (queue -> head == NULL) {
        return NULL;
    }

    Node *popped = queue -> head;
    pthread_mutex_lock(&queue_lock);
    queue -> head = popped -> next;
    if (queue -> head == NULL)
        queue -> rear = NULL;
    pthread_mutex_unlock(&queue_lock);
    return popped;
}

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(Queue *queue, Task t) 
{
    Node *new_n = (Node *) malloc(sizeof(Node));
    if (new_n == NULL) {
        return 1;
    }
    new_n -> next = NULL;
    new_n -> task = t;

    if (queue -> head == NULL) {
        pthread_mutex_lock(&queue_lock);
        queue -> head = new_n;
        queue -> rear = new_n;
        pthread_mutex_unlock(&queue_lock);
        return 0;
    }

    pthread_mutex_lock(&queue_lock);
    queue -> rear -> next = new_n;
    queue -> rear = new_n;
    pthread_mutex_unlock(&queue_lock);
    return 0;
}
