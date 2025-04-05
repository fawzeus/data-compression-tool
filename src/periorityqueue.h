#ifndef PERIORITY_QUEUE_H
#define PERIORITY_QUEUE_H

#include "types.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

errorId_t isEmpty(periorityQueue queue, bool* isEmptyQueue);
errorId_t push(periorityQueue* queue,char key, uint32 val);
errorId_t pop(periorityQueue* queue, queueNode* topNode);
errorId_t top(periorityQueue queue, queueNode* topNode);
errorId_t size(periorityQueue queue, uint32* dataLen);
errorId_t print(periorityQueue queue);
errorId_t createPeriorityQueue(periorityQueue* queue, mapNode* count[256]);
errorId_t freeQueue(periorityQueue* queue);

#endif