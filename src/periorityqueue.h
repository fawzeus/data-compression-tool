#ifndef PERIORITY_QUEUE_H
#define PERIORITY_QUEUE_H

#include "types.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Check whether the priority queue is empty.
 *
 * @param queue: The priority queue to check.
 * @param isEmptyQueue: Output flag that will be set to true if empty, false otherwise.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t isEmpty(periorityQueue queue, bool* isEmptyQueue);

/**
 * @brief Push a new Huffman node into the priority queue.
 *
 * @param queue: Pointer to the priority queue.
 * @param node: Node to insert into the queue.
 * @param doHeapifying: If true, the queue is heapified after insertion.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t push(periorityQueue* queue, huffmanNode* node, bool doHeapifying);

/**
 * @brief Remove the top (lowest count) element from the priority queue.
 *
 * @param queue: Pointer to the priority queue.
 * @param topNode: Output pointer to the removed top node.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t pop(periorityQueue* queue, huffmanTree* topNode);

/**
 * @brief Access the top (lowest count) node in the queue without removing it.
 *
 * @param queue: The priority queue.
 * @param topNode: Output pointer to the top node.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t top(periorityQueue queue, huffmanTree* topNode);

/**
 * @brief Get the current size (number of elements) in the priority queue.
 *
 * @param queue: The priority queue.
 * @param dataLen: Output parameter for the number of elements.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t getQueueSize(periorityQueue queue, uint32* dataLen);

/**
 * @brief Print the contents of the priority queue for debugging.
 *
 * @param queue: The priority queue.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t print(periorityQueue queue);

/**
 * @brief Create a priority queue from a frequency count table.
 *
 * @param queue: Pointer to the priority queue to be initialized.
 * @param count: An array of mapNode pointers representing ASCII character frequencies.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t createPeriorityQueue(periorityQueue* queue, mapNode* count[256]);

/**
 * @brief Free all memory associated with the priority queue.
 *
 * @param queue: Pointer to the queue to destroy.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t freeQueue(periorityQueue* queue);

#endif // PERIORITY_QUEUE_H