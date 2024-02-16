#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

// Define return error value.
#define ERR -1
#define SUCC 0

static queue_t threadQueue; // Global queue to manage all threads

struct uthread_tcb {
	uthread_ctx_t context;      // Execution context
    bool exited;           // Flag indicating whether the thread has exited

    void *stack;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	struct uthread_tcb *currentThread;
    printf("here1\n");
    if (queue_dequeue(threadQueue, (void **)&currentThread) == 0) {
        // Re-enqueue the current thread to maintain the queue state
        queue_enqueue(threadQueue, currentThread);
        return currentThread;
    }
    // No thread is running
    return NULL;
}

// Save the current thread and load the next thread from ready queue
void uthread_yield(void)
{
	/* TODO Phase 2 */
    struct uthread_tcb *currentThread = uthread_current();
	struct uthread_tcb *nextThread;

    // Load the next thread in ready queue
    int loaded = queue_dequeue(threadQueue, (void **)&nextThread);

    // there exist thread to yield
    if (loaded == 0)
    {
        // Save the current thread to the ready queue
        queue_enqueue(threadQueue, currentThread);

        // Switch the contexts of the old and new thread
        uthread_ctx_switch(&(currentThread->context), &(nextThread->context));
    }
}

// Mark the current thread as exited and yield to the next thread
void uthread_exit(void)
{
	/* TODO Phase 2 */
    struct uthread_tcb *currentThread = uthread_current();
    currentThread->exited = true;
    uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
    int vaild;
    struct uthread_tcb* NewThread;

    NewThread = (struct uthread_tcb *)malloc(sizeof(struct uthread_tcb));
    if (NewThread == NULL) 
    {
        // Fail to create new thread
        return ERR;
    }

    vaild = uthread_ctx_init(&(NewThread->context), uthread_ctx_alloc_stack(), func, arg);
    if (vaild == ERR) 
    {
        // Fail to initialize new thread's execution context
        free(NewThread);
        return ERR;
    }

    // No error in creating new thread, save it to ready queue
    NewThread->exited = false;
    queue_enqueue(threadQueue, NewThread);

    return SUCC;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */ 
    queue_t threadsRunning = queue_create();
    queue_t threadsExited = queue_create();
    struct uthread_tcb *mainThread;
    struct uthread_tcb *currentThread;
    int vaild;

    // Set up preemption if preempt is true
    if (preempt) 
    {
        preempt_start(true);
    }
    
    mainThread = (struct uthread_tcb *)malloc(sizeof(struct uthread_tcb));
    if (!mainThread) 
    {
        // Memory allocation failure
        return ERR;
    }

    vaild = uthread_ctx_init(&(mainThread->context), uthread_ctx_alloc_stack(), func, arg);
    if (vaild == ERR) 
    {
        // Context initialization failure
        free(mainThread);
        return ERR;  
    }

    mainThread->exited = false;
    queue_enqueue(threadsRunning, mainThread);

    // Start the scheduling loop
    while(queue_dequeue(threadsRunning, (void **)&currentThread) == 0  || queue_length(threadsRunning) > 0) 
    {
        if (!currentThread->exited) 
        {
            queue_enqueue(threadsRunning, currentThread);
            uthread_ctx_switch(&(mainThread->context), &(currentThread->context));
        } 
        else 
        {
            // Move exited thread to the exited queue
            queue_enqueue(threadsExited, currentThread);
        }
    }

    // Stop preemption if it was started
    if (preempt) {
        preempt_stop();
    }

    // Clean up
    queue_destroy(threadsRunning);
    queue_destroy(threadsExited);
    free(mainThread);

    return SUCC;  // Success
	
}

void uthread_block(void)
{
	/* TODO Phase 3 */
    //struct uthread_tcb *currentThread = uthread_current();
    //currentThread->exited = true;
    //uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
    if (uthread != NULL) {
        uthread->exited = false;
        queue_enqueue(threadQueue, uthread);
    }
}

