#include "periorityqueue.h"
#include "logger.h"

/*******************************  Static functions ********************************/

static errorId_t heapify(periorityQueue* queue) {
    const char fName[] = "heapify";
    errorId_t error = SUCCESS;
    bool isSorted = false;
    logEnter(fName);
    /* if the queue is empty its already sorted */
    if (queue == NULL || (*queue)->next == NULL) {
        return error;
    }
    while ((isSorted == false) && (error == SUCCESS)) {
        isSorted = true;
        queueNode* currentNode = *queue;
        /* swap first two element if not sorted */
        if((*queue)->val->count > (*queue)->next->val->count) {
            isSorted = false;
            queueNode auxilairyNode;
            auxilairyNode.val = (*queue)->val;
            (*queue)->val = (*queue)->next->val;
            (*queue)->next->val = auxilairyNode.val;
        }
        while ((currentNode->next != NULL) && (error == SUCCESS)) {
            if (currentNode->val->count > currentNode->next->val->count) {
                isSorted = false;
                queueNode auxilairyNode;
                auxilairyNode.val = currentNode->val;
                currentNode->val = currentNode->next->val;
                currentNode->next->val = auxilairyNode.val;
            }
            currentNode = currentNode->next;
        }
    }
    logLeave(fName);
    return error;
}

/*******************************  Public functions *********************************/
errorId_t isEmpty(periorityQueue queue, bool* isEmptyQueue){
    const char fName[] = "iSEmpty";
    errorId_t error = SUCCESS;
    logEnter(fName);
    if (isEmptyQueue == NULL) {
        error = NULL_POINTER_ERROR;
    }
    if (error == SUCCESS){
        *isEmptyQueue = (queue == NULL) ? true:false;
    }
    logLeave(fName);
    return error;
}

errorId_t push(periorityQueue* queue, huffmanNode* node) {
    const char fName[] = "push";
    errorId_t status = SUCCESS;
    bool isEmptyQueue = false;
    logEnter(fName);
    status = isEmpty(*queue,&isEmptyQueue);
    if ((status == SUCCESS) && (isEmptyQueue == true)) {
        *queue = (queueNode*) malloc(sizeof(queueNode));
        (*queue)->next = NULL;
        (*queue)->val = node;
    } else  if (isEmptyQueue == false){
        queueNode* newNode = (queueNode*) malloc (sizeof(queueNode));
        newNode->next = *queue;
        newNode->val = node;
        *queue = newNode;
    }
    if (status == SUCCESS) {
        status = heapify(queue);
    }
    logLeave(fName);
    return status;
}
errorId_t top(periorityQueue queue, huffmanTree* topNode) {
    const char fName[] = "top";
    errorId_t error = SUCCESS;
    logEnter(fName);
    if ((queue == NULL)) {
        error = NULL_POINTER_ERROR;
    }
    if (error == SUCCESS) {
        *topNode = queue->val;
    }
    logLeave(fName);
    return error;
}

errorId_t pop(periorityQueue* queue, huffmanTree* topNode) {
    const char fName[] = "pop";
    errorId_t status = SUCCESS;
    logEnter(fName);
    if((NULL == *queue) || (NULL == topNode)) {
        status = NULL_POINTER_ERROR;
    }
    if (status == SUCCESS) {
        queueNode* toBeDeleted = *queue;
        *topNode = (*queue)->val;
        (*queue) = (*queue)->next;
        free(toBeDeleted);
    }
    logLeave(fName);
    return status;
}

errorId_t getQueueSize(periorityQueue queue, uint32* dataLen) {
    const char fName[] = "size";
    errorId_t err = SUCCESS;
    logEnter(fName);
    queueNode* currentNode = queue;
    *dataLen = 0;
    uint32 size = 0;
    while (currentNode != NULL) {
        size++;
        currentNode = currentNode->next;
    }
    *dataLen = size;
    logLeave(fName);
    return err;
}

errorId_t print(periorityQueue queue) {
    const char fName[] = "print";
    errorId_t err = SUCCESS;
    queueNode* currentNode = queue;
    logEnter(fName);
    while (currentNode != NULL) {
        printf("%c: %u\n",*currentNode->val->ascii,currentNode->val->count);
        currentNode = currentNode->next;
    }
    logLeave(fName);
    return err;
    
}

errorId_t createPeriorityQueue(periorityQueue* queue, mapNode* count[256]) {
    const char fName[] = "createPeriorityQueue";
    errorId_t status = SUCCESS;
    int mapIndex = 0;
    logEnter(fName);
    for (mapIndex = 0; (mapIndex < 256) && (SUCCESS == status); mapIndex++) {
        if(count[mapIndex] != NULL) {
            huffmanNode* newNode = NULL;
            newNode = (huffmanNode*) malloc(sizeof(huffmanNode));
            if (newNode == NULL) {
                status = NULL_POINTER_ERROR;
            }
            if(status == SUCCESS) {
                newNode->ascii = (char*) malloc(sizeof(char));
            }
            if (status == SUCCESS) {
                *newNode->ascii = count[mapIndex]->key;
                newNode->count = count[mapIndex]->val;
                newNode->leftChild = NULL;
                newNode->rightChild = NULL;
            }
            if (status == SUCCESS) {
                status = push(queue, newNode);
            }
        }
    }
    logLeave(fName);
    return status;
}

errorId_t freeQueue(periorityQueue* queue){
    const char fName[] = "freeQueue";
    errorId_t status = SUCCESS;
    queueNode* currentNode = *queue;
    logEnter(fName);
    while (currentNode){
        queueNode* toBeDeleted = currentNode;
        currentNode = currentNode->next;
        free(toBeDeleted);
    }
    logLeave(fName);
    return status;  
}