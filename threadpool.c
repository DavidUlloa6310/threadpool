/**
 * Implementation of thread pool.
 */
#include "threadpool.h"

sem_t tasks_sem;
int NUM_THREADS;
int cancel_threads = 0;

Queue* queue;
pthread_t *bees;


// Worker program waits for work to be available and calls execute function to run task
void *worker(void *param)
{
    while (1) {
        if (cancel_threads == 1) {
            break;
        }

        int result = sem_wait(&tasks_sem);
        if (result < 0) {
            // Failed to wait for resource
            continue;
        } else if (result == 0) {
            Node* popped = dequeue(queue);
            if (popped != NULL) {
                Task task = popped -> task;
                execute(task.function, task.data);
            }
        }
    }
    pthread_exit(0);
}

// Execute function runs task
void execute(void (*somefunction)(void *p), void *p)
{
    printf("Executing code...\n");
    (*somefunction)(p);
}

// Allows user to submit task into thread pool - task must include function and parameters
int pool_submit(void (*somefunction)(void *p), void *p)
{
    Task *task = malloc(sizeof(Task));
    task->function = somefunction;
    task->data = p;

    int result = enqueue(queue, *task);
    if (queue -> head == NULL) {
        fprintf(stderr, "Failed to insert into queue; result = %d\n", result);
    }
    if (result == 0) {
        sem_post(&tasks_sem);
    }
    return result;
}

// Initializes pool, queue, and threads
void pool_init(int num_threads)
{
    NUM_THREADS = num_threads;
    queue = queue_init();
    bees = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    if (bees == NULL) {
        fprintf(stderr, "Memory allocation for `bees` failed");
        exit(1);
    }

    sem_init(&tasks_sem, 0, 0);
    for (int i = 0; i < num_threads; ++i)
        pthread_create(&bees[i],NULL,worker,NULL);
}

// Closes thread pool by updating cancel_threads
void pool_shutdown(void)
{
    cancel_threads = 1;
    for (int i = 0; i < NUM_THREADS; ++i) {
        sem_post(&tasks_sem);
    }

    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(bees[i],NULL);
}
