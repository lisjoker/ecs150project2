#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <uthread.h>
#include <preempt.c>
#include <private.h>


static void thread5(void *arg)
{
	(void)arg;
	printf("it's thread5,eixt\n");
	exit(0);
} 
static void thread4(void *arg)
{
	(void)arg;
	uthread_create(thread5, NULL);
	while (1) {
		
		printf("thread4 is working\n");
		uthread_yield(); 
        
    }
} 
static void thread3(void *arg)
{
	(void)arg;
	uthread_create(thread4, NULL);
	while (1) {
		
		printf("thread3 is working\n");
		sleep(1);
        
    }
}
static void thread2(void *arg)
{
	(void)arg;
	uthread_create(thread3, NULL);
	while (1) {
		sleep(1);
		printf("thread2 is working\n");
        
    }
}

static void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	while (1) {
		sleep(1);
		printf("thread1 is working\n");
        
    }
}

int main(void)
{
	uthread_run(true,thread1, NULL);
	return 0;
}
