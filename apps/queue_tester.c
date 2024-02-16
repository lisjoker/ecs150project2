#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{

	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	queue_t q;
	int data = 3, *ptr;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}
/* Enque then Destroy*/
void test_queue_destroy(void){
	queue_t q;
	fprintf(stderr, "*** TEST queue_destroy ***\n");
	q = queue_create();
	TEST_ASSERT(queue_destroy(q) != 1);
}
static void iterator_inc(queue_t q, void *data)
{
	queue_t q;
    int *a = (int*)data;

    if (*a == 6)
        queue_delete(q, data);
    else
        *a += 1;
} /*code from project2 slide*/

static void iterator_dec(queue_t q, void *data)
{
	queue_t q;
    int *a = (int*)data;

    if (*a == 6)
        queue_delete(q, data);
    else
        *a -= 1;
} /*code from project2 slide*/

void test_iterator_inc(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    assert(data[0] == 2);
    assert(queue_length(q) == 9);
}
void test_iterator_dec(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    assert(data[0] == 2);
    assert(queue_length(q) == 9);
}
void test_length(void) {
    int data[3] = {1, 2, 3};
    queue_t q;

    fprintf(stderr, "*** TEST length ***\n");

    q = queue_create();
    queue_enqueue(q, (void *)&data[0]);
    queue_enqueue(q, (void *)&data[1]);
    queue_enqueue(q, (void *)&data[2]);
    TEST_ASSERT(queue_length(q) == 3);

    // No need to store the dequeued value in ptr, just call the function
    queue_dequeue(q, NULL);
    TEST_ASSERT(queue_length(q) == 2);

    queue_dequeue(q, NULL);
    TEST_ASSERT(queue_length(q) == 1);

    queue_dequeue(q, NULL);
    TEST_ASSERT(queue_length(q) == 0);
}



void test_iterator_dec(void){
	queue_t q;
	int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int i;
	fprintf(stderr, "*** TEST queue_iterator_dec ***\n");
	q = queue_create();
	for(i = 0; i < sizeof(data) / sizeof(data[0]); i++){
		queue_enqueue(q, &data[i]);
	}
	queue_iterate(q, iterator_dec);
	printf("%d\n", queue_length(q));
	TEST_ASSERT(queue_length(q) == 1);
	
}




int main(void)
{
	test_create();
	test_length();
	test_queue_simple();
	test_queue_destroy();
	test_iterator_inc();
	test_iterator_dec();
	return 0;
}
