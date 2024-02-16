#include <assert.h>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(TEST_ASSERT)                 \
    do {                                         \
        printf("ASSERT: " #TEST_ASSERT " ... "); \
        if (TEST_ASSERT) {                       \
            printf("PASS\n");                    \
        } else {                                 \
            printf("FAIL\n");                    \
            exit(1);                             \
        }                                        \
    } while (0)

// Create
void test_create(void) {
    fprintf(stderr, "*** TEST create ***\n");

    TEST_ASSERT(queue_create() != NULL);
}

// Enqueue/Dequeue simple
void test_queue_simple(void) {
    int data = 3, *ptr;
    queue_t q;

    fprintf(stderr, "*** TEST queue_simple ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(ptr == &data);
}

// Enqueue NULL data/queue
void test_enqueue_error(void) {
    int data = 3;
    queue_t q = NULL;

    fprintf(stderr, "*** TEST enqueue_error ***\n");

    TEST_ASSERT(queue_enqueue(q, &data) == -1);
    q = queue_create();
    TEST_ASSERT(queue_enqueue(q, NULL) == -1);
    TEST_ASSERT(queue_length(q) == 0);
}

void test_dequeue_error(void) {
    int data = 3, *ptr;
    queue_t q = NULL;

    fprintf(stderr, "*** TEST dequeue_error ***\n");

    // NULL queue
    TEST_ASSERT(queue_dequeue(q, (void **)&ptr) == -1);
    q = queue_create();
    // Empty queue
    TEST_ASSERT(queue_dequeue(q, (void **)&ptr) == -1);
    queue_enqueue(q, &data);
    // NULL data
    TEST_ASSERT(queue_dequeue(q, NULL) == -1);
}

void test_delete_error(void) {
    int data = 3, data2 = 4;
    queue_t q = NULL;

    fprintf(stderr, "*** TEST delete_error ***\n");

    // NULL queue
    TEST_ASSERT(queue_delete(q, &data) == -1);
    q = queue_create();
    // Empty queue
    TEST_ASSERT(queue_delete(q, &data) == -1);
    queue_enqueue(q, &data);
    // NULL data
    TEST_ASSERT(queue_delete(q, NULL) == -1);
    // Not found
    TEST_ASSERT(queue_delete(q, &data2) == -1);
}

void test_delete_head(void) {
    int data[3] = {1, 2, 3};
    int *ptr;
    queue_t q;
    q = queue_create();
    queue_enqueue(q, &data[0]);
    queue_enqueue(q, &data[1]);
    queue_enqueue(q, &data[2]);

    fprintf(stderr, "*** TEST delete_head ***\n");
    queue_delete(q, &data[0]);
    TEST_ASSERT(queue_length(q) == 2);
    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(ptr == &data[1]);
}

void test_delete_tail(void) {
    int data[3] = {1, 2, 3};
    int *ptr1, *ptr2;
    queue_t q;
    q = queue_create();
    queue_enqueue(q, &data[0]);
    queue_enqueue(q, &data[1]);
    queue_enqueue(q, &data[2]);

    fprintf(stderr, "*** TEST delete_tail ***\n");
    queue_delete(q, &data[2]);
    TEST_ASSERT(queue_length(q) == 2);
    queue_dequeue(q, (void **)&ptr1);
    queue_dequeue(q, (void **)&ptr2);
    TEST_ASSERT(ptr1 == &data[0]);
    TEST_ASSERT(ptr2 == &data[1]);
}

void test_delete_middle(void) {
    int data[3] = {1, 2, 3};
    int *ptr1, *ptr2;
    queue_t q;
    q = queue_create();
    queue_enqueue(q, &data[0]);
    queue_enqueue(q, &data[1]);
    queue_enqueue(q, &data[2]);

    fprintf(stderr, "*** TEST delete_middle ***\n");
    queue_delete(q, &data[1]);
    TEST_ASSERT(queue_length(q) == 2);
    queue_dequeue(q, (void **)&ptr1);
    queue_dequeue(q, (void **)&ptr2);
    TEST_ASSERT(ptr1 == &data[0]);
    TEST_ASSERT(ptr2 == &data[2]);
}

void test_length(void) {
    int data[3] = {1, 2, 3};
    int *ptr;
    queue_t q;

    fprintf(stderr, "*** TEST length ***\n");

    q = queue_create();
    queue_enqueue(q, &data[0]);
    queue_enqueue(q, &data[1]);
    queue_enqueue(q, &data[2]);
    TEST_ASSERT(queue_length(q) == 3);

    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(queue_length(q) == 2);

    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(queue_length(q) == 1);

    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(queue_length(q) == 0);
}

void test_destroy_error(void) {
    int data = 1;
    int *ptr;
    queue_t q = NULL;

    fprintf(stderr, "*** TEST destroy_error ***\n");

    // Null queue
    TEST_ASSERT(queue_destroy(q) == -1);

    // Not empty queue
    q = queue_create();
    queue_enqueue(q, &data);
    TEST_ASSERT(queue_destroy(q) == -1);

    // Empty queue
    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(queue_destroy(q) == 0);
}

/* Callback function that increments items */
static void iterator_inc(queue_t q, void *data) {
    int *a = (int *)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

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

int main(void) {
    test_create();
    test_queue_simple();
    test_enqueue_error();
    test_dequeue_error();
    test_delete_error();
    test_delete_head();
    test_delete_tail();
    test_delete_middle();
    test_length();
    test_destroy_error();
    test_iterator();

    return 0;
}
