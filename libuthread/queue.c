#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// define success value
#define SUCC 0; 
// define error value.
#define ERR -1; 

struct queue {
	/* TODO Phase 1 */
	struct Node {
		// pointer to the next element
        struct Node *next;
		// pointer to the current element
        void *data;
    } * front, *rear;
	// front points to the first element in the linked structure
	// rear points to the last element in the linked structure
    int size;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	struct queue* new_queue = malloc(sizeof(struct queue));
    if (!new_queue) {
        // Handle memory allocation failure
        return NULL;
    }

	new_queue->front = NULL;
    new_queue->rear = NULL;
    new_queue->size = 0;

    return new_queue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if (!queue || queue->size != 0) {
        // Queue is NULL or not empty, cannot destroy
        return ERR;
    }

    free(queue);
    return SUCC;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if (!queue || !data) {
        // Invalid parameters
        return ERR;
    }

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (!new_node) {
        // Handle memory allocation failure
        return ERR;
    }

	new_node->data = data;
    new_node->next = NULL;

	if (queue->size == NULL) {
        // Queue is empty
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        // Add to the end of the queue
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    queue->size++;
    return SUCC;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
    if (!queue || !data || queue->size == 0) {
        // Invalid parameters or empty queue
        return ERR;
    }

    struct Node *temp = queue->front;
    *data = temp->data;

    queue->front = temp->next;
    free(temp);

    if (queue->front == NULL) {
        // Queue is now empty
        queue->rear = NULL;
    }

    queue->size--;
    return SUCC;
}

int queue_delete(queue_t queue, void *data) {
	// queue ore data are NULL
    if(queue == NULL || data == NULL || queue->size == 0){
        return ERR;
    }
    int found = 0;
    int queueLen = queue->size;
    struct Node* previous = malloc(sizeof(struct Node));
    struct Node* current = queue->front;
    previous = NULL;
    for(int i = 0; i < queueLen; i++){
        if(current->data == data){
            found = 1;
            // goal data is the head of the queue
            if(i == 0){
                if(queue->size == 1){ // only one element in the queue
                    queue->size--;
                    queue->front = NULL;
                    queue->rear = NULL;
                    free(current); // deallocate the goal data in queue

                } else if(queue->size > 1) { // more than one elements in the queue
                    queue->size--;

                    current = queue->front->next;// deallocate the goal data in queueQ
                    queue->front = NULL;
                    queue->front = current;
                }
            } else { // goal data is not the head of the queue
                queue->size--;
                previous->next = current->next;
                free(current); // deallocate the goal data in queue
            }
            break; // only delete the first match one
        } else { // goal data not found
            previous = current;
            current = current->next;
            previous->next = current;
        }
    }
    if(found == 0){ // data was not found
        return ERR;
    }
    return SUCC;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
    if (!queue || !func || queue->size == 0) {
        // Invalid parameters
        return ERR;
    }

    struct Node *current = queue->front;
    while (current != NULL) {
        // Call the provided function on each data item
        func(queue, current->data);
        current = current->next;
    }

    return SUCC;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
    if (!queue) {
        // Invalid parameter
        return ERR;
    }

    return queue->size;
}

