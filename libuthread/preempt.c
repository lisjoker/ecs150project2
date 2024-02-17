#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
static struct sigaction old_action;
static struct itimerval timer;
static bool preempt_enabled = false;

static void preempt_signal_handler(int signum) {
    if (signum == SIGVTALRM) {
        uthread_yield(); // Force the currently running thread to yield
    }
}

void preempt_disable(void) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &set, NULL); // Block SIGVTALRM signals
    preempt_enabled = false;
}

void preempt_enable(void) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &set, NULL); // Unblock SIGVTALRM signals
    preempt_enabled = true;
}

void preempt_start(bool preempt) {
    if (preempt) {
        struct sigaction action;
        struct itimerval new_timer;

        // Set up the signal handler
        action.sa_handler = preempt_signal_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGVTALRM, &action, &old_action);

        // Configure the timer
        new_timer.it_interval.tv_sec = 0;
        new_timer.it_interval.tv_usec = 1000000 / HZ;
        new_timer.it_value = new_timer.it_interval;
        setitimer(ITIMER_VIRTUAL, &new_timer, &timer);
    }
}

void preempt_stop(void) {
    if (preempt_enabled) {
        // Restore the old signal handler
        sigaction(SIGVTALRM, &old_action, NULL);

        // Disable the timer
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;
        timer.it_value = timer.it_interval;
        setitimer(ITIMER_VIRTUAL, &timer, NULL);
    }
}
