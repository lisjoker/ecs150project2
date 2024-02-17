#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
// define success value
#define SUCC 0
// Define error value.
#define ERR -1
struct Node
{
	// pointer to the next element
    struct Node *nextNode;
	// pointer to the current element
    void *data;
};
struct queue
{
	/* TODO Phase 1 */
    // front points to the first element in the linked structure
	// rear points to the last element in the linked structure
    struct Node *front, *end;
    int size;
};
queue_t queue_create(void)
{
	/* TODO Phase 1 */
	struct queue* new_queue = malloc(sizeof(struct queue));
    if (new_queue == NULL)
    {
        // Handle memory allocation failure
        return NULL;
    }
	new_queue->front = NULL;
    new_queue->end = NULL;
    new_queue->size = 0;
    return new_queue;
}
int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if (queue == NULL || queue->size != 0)
    {
        // Queue is NULL or not empty, cannot destroy
        return ERR;
    }
    free(queue);
    return SUCC;
}
int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
    struct Node *last_node = (struct Node *)malloc(sizeof(struct Node));
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
	if ((queue == NULL) || (data == NULL) || (new_node == NULL))
    {
        // Invalid parameters or memory allocation failure
        return ERR;
    }
    // Initialize the new node
	new_node->data = data;
    new_node->nextNode = NULL;
    last_node = queue->end;
	if (queue->end == NULL || queue->size == 0)
    {
        // Queue is empty, front asnd end points to the same address
        queue->front = new_node;
    } 
    else
    {
        // Add the new node to the end of the queue
        last_node->nextNode = new_node;    // connects the new node to the existing last node
    }
    // updates the end pointer to point to the new node
    queue->end = new_node;
    queue->size++;
    // New node added successgully
    return SUCC;
}
int queue_dequeue(queue_t queue, void **data)
{
    struct Node* firstNode = malloc(sizeof(struct Node));
	/* TODO Phase 1 */
    if (queue == NULL || data == NULL || queue->size == 0 || firstNode == NULL)
    {
        // Invalid parameters or empty queue
        return ERR;
    }
    // Get the first element in queue
    firstNode = queue->front;
    // Data stores the first thing in queue
    *data = firstNode->data;
    // Relocate the queue's first element to the second element
    queue->front = firstNode->nextNode;
    // The first element of the queue is now gone
    queue->size--;
    // Check if queue still contains elements
    if (queue->size == 0)
    {
        // Queue now is empty
        queue->front = NULL;
        queue->end = NULL;
    }
    return SUCC;
}
// Delete data from the queue
int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
    if ((queue == NULL) || (data == NULL) || (queue->size == 0))
    {
        // Invalid parameters or quque is empty
        return ERR;
    }
    struct Node *current = queue->front;
    struct Node *previous = NULL;
    while (current != NULL)
    {
        // Checking for match
        if (current->data == data)
        {
            // Found the data, remove it from the queue
            if (previous == NULL)
            {
                // Deleting the front node
                queue->front = current->nextNode;
                if (queue->front == NULL)
                {
                    // Queue is now empty
                    queue->end = NULL;
                }
            } 
            else
            {
                // Deleting a node in the middle or end
                previous->nextNode = current->nextNode;
                if (previous->nextNode == NULL)
                {
                    // Deleted the last node, update end
                    queue->end = previous;
                }
            }
            queue->size--;
            return SUCC;
        }
        // Update current node
        previous = current;
        current = current->nextNode;
    }
    // Data not found in the queue
    return ERR;
}
int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
    if ((queue == NULL) || (func == NULL) || (queue->size == 0))
    {
        // Invalid parameters
        return ERR;
    }
    struct Node *current = queue->front;
    while (current != NULL)
    {
        // Call the provided function on each data item
        func(queue, current->data);
        current = current->nextNode;
    }
    return SUCC;
}
int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
    if (!queue)
    {
        // Invalid parameter
        return ERR;
    }
    return queue->size;
}
