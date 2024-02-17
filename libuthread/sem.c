#include <stddef.h>
#include <stdlib.h>
#include "queue.h"
#include "sem.h"
#include "private.h"
#include "uthread.h"
// Define return error value.
#define ERR -1
#define SUCC 0
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
	int vaild;
	if ((sem == NULL) || (queue_length(sem->waitQueue) == 0))
	{
		// Invalid semaphore or threads still blocked
        return ERR;
    }
    vaild = queue_destroy(sem->waitQueue);
	if(vaild == ERR)
	{
		// There are still threads in queue
		return ERR;
	}
    free(sem);
	// Successful destruction
    return SUCC;
}
int sem_down(sem_t sem) 
{
	/* TODO Phase 3 */
    if (sem == NULL) 
	{
        return ERR; // Invalid semaphore
    }
	while (sem->count == 0)
	{
		if (sem->count > 0) 
		{
			struct uthread_tcb *newThread = uthread_current();
			sem->count--;
			// Unblock the previously blocked thread
			uthread_unblock(newThread);
		}
		else 
		{
			// Block the current thread
			uthread_block();
		}
	}
	sem->count--;
	// Successful down operation
    return SUCC;
}
int sem_up(sem_t sem) 
{
    if (sem == NULL)
	{
        return ERR; // Invalid semaphore
    }
	int vaild;
    sem->count++;
	// waiting list associated to @sem is not empty
    if (queue_length(sem->waitQueue) == 0)
	{
        // Unblock the first thread from the wait_queue
        struct uthread_tcb *blocked_thread;
		// Dequeuing the first thread from queue
        vaild = queue_dequeue(sem->waitQueue, (void **)&blocked_thread);
		if (vaild == ERR)
		{
			return ERR;
		}
		sem->blockedCtx = NULL;
        uthread_unblock(blocked_thread);
    }
	// Successful up operation
    return SUCC;
}
