#include "uthread.h"
#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "queue.h"

// Define return error value.
#define ERR -1
#define SUCC 0

queue_t readyThreadsQueue;

struct uthread_tcb *currThread;

struct uthread_tcb {
    uthread_ctx_t context;
    void *stack;
};

struct uthread_tcb *uthread_current(void) {
    /* TODO Phase 2/3 */
    return currThread;
}

void uthread_yield(void) {
    struct uthread_tcb *oldThread = uthread_current();
    int vaild;

    // Save the old thread to queue
    queue_enqueue(readyThreadsQueue, oldThread);
    // Load the new thred from queue
    vaild = queue_dequeue(readyThreadsQueue, (void **)&currThread);

    // Queue is not empty
    if (vaild == SUCC) 
    {
        // Switch the context between old and new thread
        uthread_ctx_switch(&(oldThread->context), &(currThread->context));
    }
}

// Like the yield function but without enqueue. 
void uthread_exit(void) {
    struct uthread_tcb *oldThread = uthread_current();
    int vaild;

    // Remove the thread from the queue
    vaild = queue_dequeue(readyThreadsQueue, (void **)&currThread);

    // Queue is not empty
    if (vaild == SUCC)
    {
        // Destroy the stack of that thread
        uthread_ctx_destroy_stack(uthread_ctx_alloc_stack());

        // Remove the context of that thread
        uthread_ctx_switch(&(oldThread->context), &(currThread->context));
    }
}

int uthread_create(uthread_func_t func, void *arg) {
    struct uthread_tcb *newThread; 
    int vaild;

    newThread = malloc(sizeof(struct uthread_tcb));
    if (newThread == NULL) 
    {
        // Fail to create new thread
        return ERR;
    }

    vaild = uthread_ctx_init(&(newThread->context), uthread_ctx_alloc_stack(), func, arg);
    if (vaild == ERR) 
    {
        // Memory allocation failure
        return ERR;
    }

    // Success creating new thread, add it to ready queue
    queue_enqueue(readyThreadsQueue, newThread);

    return SUCC;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) {
    	/* TODO Phase 2 */
    struct uthread_tcb *oldThread;
    struct uthread_tcb *idleThread;
    int vaild;

    // Initialize the queue
    readyThreadsQueue = queue_create();

    idleThread = malloc(sizeof(struct uthread_tcb));
    if (idleThread == NULL) {
        return ERR;
    }

    vaild = uthread_ctx_init(&(idleThread->context), uthread_ctx_alloc_stack(), func, arg);
    if (vaild == ERR) 
    {
        // Memory allocation failure
        return ERR;
    }

    // Success creating new thread, add it to ready queue
    queue_enqueue(readyThreadsQueue, idleThread);

    while (queue_length(readyThreadsQueue) > 0) 
    {
        oldThread = uthread_current();

        // Save the old thread to queue
        queue_enqueue(readyThreadsQueue, oldThread);
        // Load the new thred from queue
        vaild = queue_dequeue(readyThreadsQueue, (void **)&idleThread);

        // Queue is not empty
        if (vaild == SUCC) 
        {
            // Switch the context between old and new thread
            uthread_ctx_switch(&(oldThread->context), &(idleThread->context));
        }
    }

    queue_destroy(readyThreadsQueue);

    return SUCC;
}

void uthread_block(void) {
    /* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread) {
    /* TODO Phase 3 */
}
