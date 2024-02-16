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

queue_t readyThreads;
queue_t runningThreads;
queue_t blockedThreads;
queue_t zombieThreads;

struct uthread_tcb *currThread;

struct uthread_tcb {
    int state;
    uthread_ctx_t context;
    void *stack;
};

struct uthread_tcb *uthread_current(void) {
    return currThread;
}

void uthread_yield(void) {
    struct uthread_tcb *prev = uthread_current();
    struct uthread_tcb *next;

    queue_enqueue(readyThreads, currThread);
    queue_dequeue(readyThreads, (void **)&currThread);
    next = currThread;
    uthread_ctx_switch(&(prev->context), &(next->context));
}

void uthread_exit(void) {
    struct uthread_tcb *prev = uthread_current();
    struct uthread_tcb *next;

    uthread_ctx_destroy_stack(currThread->stack);
    queue_dequeue(readyThreads, (void **)&currThread);
    next = currThread;
    uthread_ctx_switch(&(prev->context), &(next->context));
}

int uthread_create(uthread_func_t func, void *arg) {
    struct uthread_tcb *newThread = malloc(sizeof(struct uthread_tcb));
    if (newThread == NULL) {
        return -1;
    }

    newThread->stack = uthread_ctx_alloc_stack();
    if (newThread->stack == NULL) {
        return -1;
    }

    if (uthread_ctx_init(&(newThread->context), newThread->stack, func, arg) ==
        -1) {
        return -1;
    }
    queue_enqueue(readyThreads, newThread);

    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) {
    readyThreads = queue_create();
    struct uthread_tcb *idleThread = malloc(sizeof(struct uthread_tcb));
    if (idleThread == NULL) {
        return -1;
    }
    currThread = idleThread;
    uthread_create(func, arg);

    // printf("here\n");
    while (queue_length(readyThreads) > 0) {
        // printf("here2\n");
        // printf("%d\n", queue_length(readyThreads));
        uthread_yield();
        // printf("here3\n");
    }
    // printf("done\n");
    queue_destroy(readyThreads);
    return 0;
}

void uthread_block(void) {
    /* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread) {
    /* TODO Phase 3 */
}
