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

// static queue_t readyThreads;
// static queue_t runningThreads;
// static queue_t blockedThreads;

struct uthread_tcb *currentThread;

static queue_t threadQueue; // Global queue to manage all threads

struct uthread_tcb {
	uthread_ctx_t context;      // Execution context
    bool exited;           // Flag indicating whether the thread has exited
    void *stack;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
    /*/
	struct uthread_tcb *currentThread;
    if (queue_dequeue(threadQueue, (void **)&currentThread) == 0) {
        // Re-enqueue the current thread to maintain the queue state
        queue_enqueue(threadQueue, currentThread);
        return currentThread;
    }
    // No thread is running
    return NULL;
    */
    return currentThread;
}

// Save the current thread and load the next thread from ready queue
void uthread_yield(void)
{
	/* TODO Phase 2 */
    struct uthread_tcb *currentThread = uthread_current();
	struct uthread_tcb *nextThread;
    /*/
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
    */
    queue_enqueue(threadQueue, currentThread);
    queue_dequeue(threadQueue, (void **)&currentThread);
    nextThread = currentThread;
    uthread_ctx_switch(&(prev->context), &(nextThread->context));
}

// Mark the current thread as exited and yield to the next thread
void uthread_exit(void)
{
	/* TODO Phase 2 */
    struct uthread_tcb *currentThread = uthread_current();
    //currentThread->exited = true;
    //uthread_yield();
    struct uthread_tcb *next;
    uthread_ctx_destroy_stack(uthread_ctx_alloc_stack());
    queue_dequeue(threadQueue, (void **)&currThread);
    next = currentThread;
    uthread_ctx_switch(&(currentThread->context), &(next->context));
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
    threadQueue = queue_create();
    struct uthread_tcb *mainThread;
    //int vaild;

    // Set up preemption if preempt is true
    /*/
    if (preempt) 
    {
        preempt_start(true);
    }
    */
    mainThread = malloc(sizeof(struct uthread_tcb));
    if (mainThread == NULL) 
    {
        // Memory allocation failure
        return ERR;
    }
    currentThread = mainThread;
    uthread_create(func, arg);
    //uthread_ctx_init(&(mainThread->context), uthread_ctx_alloc_stack(), func, arg);
    //vaild = uthread_ctx_init(&(mainThread->context), uthread_ctx_alloc_stack(), func, arg);
    //if (vaild == ERR) 
    //{
        // Context initialization failure
    //    free(mainThread);
    //    return ERR;  
    //}

    //mainThread->exited = false;
    //queue_enqueue(threadQueue, mainThread);

    // Start the scheduling loop 
    while(queue_length(threadQueue) > 0)
    {
        uthread_yield();
    }

    // Stop preemption if it was started
    /*/
    if (preempt) 
    {
        preempt_stop();
    }
    */
    queue_destroy(threadQueue);
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

