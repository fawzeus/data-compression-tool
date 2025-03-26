#include "periorityqueue.h"
#include <stddef.h>

/* public functions */

errorId_t isEmpty(periorityQueue queue, bool* isEmptyQueue){
    errorId_t error = SUCCESS;
    if (isEmptyQueue == NULL) {
        error = NULL_POINTER_ERROR;
    }
    if (error == SUCCESS){
        *isEmptyQueue = (queue == NULL) ? true:false;
    }
}

errorId_t push(periorityQueue* queue, char key, uint32 val) {
    errorId_t status = SUCCESS;
    bool isEmptyQueue = false;
    status = isEmpty(queue,&isEmptyQueue);
    if ((status == SUCCESS) && (isEmptyQueue == true)) {
        *queue = (queueNode*) malloc (sizeof(queueNode));
        (*queue)->next = NULL;
        (*queue)->key = key;
        (*queue)->val = val;
    } else  if (isEmptyQueue == false){
        queueNode* newNode = (queueNode*) malloc (sizeof(queueNode));
        newNode->next = *queue;
        newNode->key = key;
        newNode->val = val;
        queue = newNode;
    }
    status = heapify(queue);
    return status;
}
errorId_t top(periorityQueue queue, queueNode* topNode) {
    errorId_t error = SUCCESS;
    if ((queue == NULL) || (topNode == NULL)) {
        error = NULL_POINTER_ERROR;
    }
    if (error == SUCCESS) {
        topNode->key = queue->key;
        topNode->val = queue->val;
        topNode->next = NULL;
    }
    return error;
}

errorId_t pop(periorityQueue* queue, queueNode* topNode) {
    errorId_t status = SUCCESS;
    if((NULL == *queue) || (NULL == topNode)) {
        status = NULL_POINTER_ERROR;
    }
    if (status == SUCCESS) {
        queueNode* toBeDeleted = *queue;
        topNode->key = (*queue)->key;
        topNode->val = (*queue)->val;
        topNode->next = NULL;
        (*queue) = (*queue)->next;
        free(toBeDeleted);
    }
    return status;
}

errorId_t size(periorityQueue queue, uint32* dataLen) {
    errorId_t err = SUCCESS;
    queueNode* currentNode = queue;
    *dataLen = 0;
    while (currentNode != NULL) {
        *dataLen++;
        currentNode = currentNode->next;
    }
    return err;
}

/* Static functions */
static errorId_t heapify(periorityQueue* queue) {
    errorId_t error = SUCCESS;
    bool isSorted = false;
    /* if the queue is empty its already sorted */
    if (queue == NULL) {
        return error;
    }
    while ((isSorted == false) && (error == SUCCESS)) {
        isSorted = true;
        queueNode* currentNode = *queue;
        /* swap first two element if not sorted */
        if((*queue)->val > (*queue)->next->val) {
            isSorted = false;
            queueNode auxilairyNode;
            auxilairyNode.key = (*queue)->key;
            auxilairyNode.val = (*queue)->val;
            (*queue)->key = (*queue)->next->key;
            (*queue)->val = (*queue)->next->val; 
            (*queue)->next->val = auxilairyNode.val;
            (*queue)->next->key = auxilairyNode.key;
        }
        while ((currentNode->next != NULL) && (error == SUCCESS)) {
            if (currentNode->val > currentNode->next->val) {
                isSorted = false;
                queueNode auxilairyNode;
                auxilairyNode.key = currentNode->key;
                auxilairyNode.val = currentNode->val;
                currentNode->key = currentNode->next->key;
                currentNode->val = currentNode->next->val;
                currentNode->next->key = auxilairyNode.key;
                currentNode->next->val = auxilairyNode.val;
            }
            currentNode = currentNode->next;
        }
        
    }
    
    return error;
}