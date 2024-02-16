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

	if (queue->rear == 0) {
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

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
    if (!queue || !data || queue->size == 0) {
        // Invalid parameters
        return ERR;
    }

    struct Node *current = queue->front;
    struct Node *previous = NULL;

    while (current != NULL) {
        if (current->data == data) {
            // Found the data, remove it from the queue
            if (previous == NULL) {
                // Deleting the front node
                queue->front = current->next;
                free(current);

                if (queue->front == NULL) {
                    // Queue is now empty
                    queue->rear = NULL;
                }
            } else {
                // Deleting a node in the middle or end
                previous->next = current->next;
                free(current);

                if (previous->next == NULL) {
                    // Deleted the last node, update rear
                    queue->rear = previous;
                }
            }

            queue->size--;
            return SUCC;
        }

        previous = current;
        current = current->next;
    }

    // Data not found in the queue
    return ERR;
}

int queue_iterate(queue_t queue, queue_func_t func) {
	if(queue == NULL || func == NULL || queue->size == 0){
        return ERR;
    }
    // iterate from head of the queue
    struct Node* current = queue->front;
    struct Node* temp = malloc(sizeof(struct Node));
    while(current != NULL){
        // go through each node in the queue
        temp = current;
        current = current->next;
        // do function for each node
        func(temp->data);
    }
    temp = NULL;
    free(temp);
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

