#include <assert.h>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>

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

void test_length(void) {
    int data[5] = {1, 2, 3,4,5};
    int *ptr;
    queue_t q;

    fprintf(stderr, "*** TEST length ***\n");

    q = queue_create();
    queue_enqueue(q, &data[0]);
    queue_enqueue(q, &data[1]);
    queue_enqueue(q, &data[2]);
	queue_enqueue(q, &data[4]);
    queue_enqueue(q, &data[5]);
   
    TEST_ASSERT(queue_length(q) == 5);

    for (; queue_length(q) > 0; ) {
    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(queue_length(q) == (queue_length(q) - 1));
}
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
static void iterator_inc(queue_t q, void *data) {
    int *a = (int *)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}
/*code form slide*/
void test_iterator(void) {
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    fprintf(stderr, "*** TEST iterator ***\n");

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);
}



int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_destroy();
	test_iterator();
	test_length();

	return 0;
}
