#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

// Define return error value.
#define ERR -1
#define SUCC 0

struct semaphore 
{
	/* TODO Phase 3 */
	size_t count;	// internal count
    queue_t waitQueue;
    //mutex_t mutex; // Mutex for protecting critical sections
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t semaphore = (sem_t)malloc(sizeof(struct semaphore));
    if (semaphore == NULL) 
	{
		// Fail to create allocate heap
        return NULL;
    }

    semaphore->count = count;
    //ueue_init(&semaphore->wait_queue);
    //mutex_init(&semaphore->mutex); // Initialize the mutex

    return semaphore;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL )//|| !queue_empty(&sem->wait_queue)) 
	{
        return ERR; // Invalid semaphore or threads still blocked
    }

    //mutex_destroy(&sem->mutex);
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

    //mutex_lock(&sem->mutex);

    while (sem->count == 0) 
	{
        // Add the calling thread to the wait_queue and block it
        uthread_block(); // Assuming this function is part of the private thread API
        queue_enqueue(&sem->waitQueue, uthread_current()); // Enqueue the blocked thread
        //mutex_unlock(&sem->mutex);
        uthread_yield(); // Yield control to another thread
        //mutex_lock(&sem->mutex);
    }

    sem->count--;

    //mutex_unlock(&sem->mutex);

    return SUCC; // Successful down operation
}

int sem_up(sem_t sem) 
{
    if (sem == NULL) 
	{
        return ERR; // Invalid semaphore
    }

    //mutex_lock(&sem->mutex);

    sem->count++;

    if (queue_length(&sem->waitQueue) == 0) 
	{
        // Unblock the first thread from the wait_queue
        struct uthread_tcb *blocked_thread;
        queue_dequeue(&sem->waitQueue, (void **)&blocked_thread);
        uthread_unblock(blocked_thread); // Assuming this function is part of the private thread API
    }

    //mutex_unlock(&sem->mutex);

    return SUCC; // Successful up operation
}
