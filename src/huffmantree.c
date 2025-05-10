#include "huffmantree.h"
#include "periorityqueue.h"
#include "logger.h"
/*********************************** Static Functions  ***********************************/
/*********************************** Public Functions  ***********************************/
errorId_t createHuffmanTree(huffmanTree* tree, periorityQueue* queue){
    const char fName[] = "createHuffmanTree";
    errorId_t status = SUCCESS;
    uint32 queueSize = 0;
    logEnter(fName);
    status = getQueueSize(*queue, &queueSize);
    while ((status == SUCCESS) && (queueSize > 1)) {
        huffmanNode* node1 = NULL;
        huffmanNode* node2 = NULL;
        huffmanNode* newNode = NULL;
        status = pop(queue,&node1);
        if(status == SUCCESS) {
            status = pop(queue,&node2);
        }
        newNode = (huffmanNode*) malloc(sizeof(huffmanNode));
        if ((status == SUCCESS) && (newNode == NULL)) {
            status = NULL_POINTER_ERROR;
        }
        if (status == SUCCESS) {
            newNode->ascii = NULL;
            newNode->count = node1->count + node2->count;
            newNode->leftChild = node1;
            newNode->rightChild = node2;
        }
        if (status == SUCCESS) {
            status = push(queue,newNode);
        }
        if (status == SUCCESS) {
            status = getQueueSize(*queue, &queueSize);
        }
    }
    if (status == SUCCESS) {
        status = top(*queue,tree);
    }
    logLeave(fName);
    return status;
}
