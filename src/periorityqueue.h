#include "types.h"

typedef struct queueNode queueNode;
typedef queueNode* periorityQueue;
struct queueNode{
    char key;
    uint32 val;
    queueNode* next;
};

errorId_t isEmpty(periorityQueue queue, bool* isEmptyQueue);
errorId_t push(periorityQueue* queue,char key, uint32 val);
errorId_t pop(periorityQueue* queue, queueNode* topNode);
errorId_t top(periorityQueue queue, queueNode* topNode);
errorId_t size(periorityQueue queue, uint32* dataLen);
