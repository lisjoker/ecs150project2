#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
#include "uthread.h"

// Define return error value.
#define ERR -1
#define SUCC 0

struct uthread_tcb *newThread;

struct semaphore 
{
	/* TODO Phase 3 */
	size_t count;	// internal count
    queue_t waitQueue;
    uthread_ctx_t* blockedCtx;  // To keep track of the blocked thread's context
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t newSemaphore = (sem_t)malloc(sizeof(struct semaphore));
    if (newSemaphore == NULL) 
	{
		// Fail to create allocate heap
        return NULL;
    }

	// Initialize the variable of the new semaphore
    newSemaphore->count = count;
    newSemaphore->blockedCtx = NULL;
    newSemaphore->waitQueue = queue_create();

    return newSemaphore;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if ((sem == NULL) || (queue_length(sem->waitQueue) == 0)) 
	{
        return ERR; // Invalid semaphore or threads still blocked
    }

    queue_destroy(sem->waitQueue);

    free(sem);

    return SUCC; // Successful destruction

}

int sem_down(sem_t sem) 
{
	/* TODO Phase 3 */
    if (sem == NULL) 
	{
        return ERR; // Invalid semaphore
    }

	// Block the current thread
    uthread_block();

    if (sem->count > 0) 
	{
		newThread = uthread_current();
        sem->count--;
		// Unblock the previously blocked thread
        uthread_unblock(newThread);
    } 
	else 
	{
		// Store the blocked thread's context
        sem->waitQueue = uthread_current();
        queue_enqueue(sem->waitQueue, sem->blockedCtx);
        uthread_block();  // Block the current thread
    }

    return SUCC; // Successful down operation
}

int sem_up(sem_t sem) 
{
    if (sem == NULL) 
	{
        return ERR; // Invalid semaphore
    }

    sem->count++;

	// waiting list associated to @sem is not empty
    if (queue_length(sem->waitQueue) == 0) 
	{
        // Unblock the first thread from the wait_queue
        struct uthread_tcb *blocked_thread;
		// Dequeuing the first thread from queue
        queue_dequeue(sem->waitQueue, (void **)&blocked_thread);
		sem->blockedCtx = NULL;
        uthread_unblock(blocked_thread);
    }

    return SUCC; // Successful up operation
}
