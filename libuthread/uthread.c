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

queue_t ThreadsQueue;

struct uthread_tcb {
    uthread_ctx_t context;
};

struct uthread_tcb *uthread_current(void) {
    // Retrieve the currently running thread from the global queue
    struct uthread_tcb *current_thread;
    if (queue_dequeue(ThreadsQueue, (void **)&current_thread) == SUCC) {
        // Re-enqueue the current thread to maintain the queue state
        queue_enqueue(ThreadsQueue, current_thread);
        return current_thread;
    }
    return NULL;
}

void uthread_yield(void) {
    struct uthread_tcb *old_thread = uthread_current();
    int valid;

    // Save the old thread to queue
    queue_enqueue(ThreadsQueue, old_thread);
    // Load the new thread from the queue
    valid = queue_dequeue(ThreadsQueue, (void **)&current_thread);

    // Queue is not empty
    if (valid == SUCC) {
        // Switch the context between old and new thread
        uthread_ctx_switch(&(old_thread->context), &(current_thread->context));
    }
}

void uthread_exit(void) {
    struct uthread_tcb *old_thread = uthread_current();
    int valid;

    // Remove the thread from the queue
    valid = queue_dequeue(ThreadsQueue, (void **)&current_thread);

    // Queue is not empty
    if (valid == SUCC) {
        // Destroy the stack of that thread
        uthread_ctx_destroy_stack(uthread_ctx_alloc_stack());

        // Remove the context of that thread
        uthread_ctx_switch(&(old_thread->context), &(current_thread->context));
    }
}

int uthread_create(uthread_func_t func, void *arg) {
    struct uthread_tcb *new_thread;
    int valid;

    new_thread = malloc(sizeof(struct uthread_tcb));
    if (new_thread == NULL) {
        // Fail to create new thread
        return ERR;
    }

    valid = uthread_ctx_init(&(new_thread->context), uthread_ctx_alloc_stack(), func, arg);
    if (valid == ERR) {
        // Memory allocation failure
        return ERR;
    }

    // Success creating new thread, add it to ready queue
    queue_enqueue(ThreadsQueue, new_thread);

    return SUCC;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) {
    struct uthread_tcb *idle_thread;
    struct uthread_tcb *old_thread, *new_thread;
    int valid1, valid2;

    // Initialize the queue
    ThreadsQueue = queue_create();

    idle_thread = malloc(sizeof(struct uthread_tcb));
    new_thread = malloc(sizeof(struct uthread_tcb));
    if ((idle_thread == NULL) || (new_thread == NULL)) {
        // Fail to create thread
        return ERR;
    }

    valid1 = uthread_ctx_init(&(idle_thread->context), uthread_ctx_alloc_stack(), func, arg);
    valid2 = uthread_ctx_init(&(new_thread->context), uthread_ctx_alloc_stack(), func, arg);
    if (valid1 == ERR || valid2 == ERR) {
        // Memory allocation failure
        return ERR;
    }

    // Success creating new thread, add it to ready queue
    queue_enqueue(ThreadsQueue, new_thread);

    while (queue_length(ThreadsQueue) > 0) {
        old_thread = uthread_current();

        // Save the old thread to queue
        queue_enqueue(ThreadsQueue, old_thread);
        // Load the new thread from the queue
        queue_dequeue(ThreadsQueue, (void **)&current_thread);
        // Switch the context between old and new thread
        uthread_ctx_switch(&(old_thread->context), &(current_thread->context));
    }

    queue_destroy(ThreadsQueue);
    return SUCC;
}

void uthread_block(void) {
    // TODO Phase 3
}

void uthread_unblock(struct uthread_tcb *uthread) {
    // TODO Phase 3
}
