#include "utils.h"
#include "periorityqueue.h"
#include "logger.h"
#include "huffmantree.h"
errorId_t createQueue(periorityQueue* queue, char* str, size_t len) {
    const char fName[] = "createQueue";
    errorId_t err = SUCCESS;
    mapNode* count[256] = {NULL};
    logEnter(fName);
    for (size_t i = 0; i < len; i++) {
        if (count[(size_t) str[i]] == NULL){
            count[(size_t) str[i]] = (mapNode*) malloc(sizeof(mapNode));
            count[(size_t) str[i]]->key = str[i];
            count[(size_t) str[i]]->val = 1;
        }
        else {
            count[(size_t) str[i]]->val++;
        }
    }
    createPeriorityQueue(queue, count);
    for (size_t i = 0; i < len; i++){
        if(count[i] != NULL) {
            free(count[i]);
        }
    }
    print(*queue);
    logLeave(fName);
    return err;
}

errorId_t encode(huffmanTree* tree, const char* filePath){
    const char fName[] = "encode";
    errorId_t status = SUCCESS;
    FILE *filePtr = NULL;
    size_t fileSize = 0;
    char* str = NULL;
    periorityQueue queue = NULL;
    logEnter(fName);
    filePtr = fopen(filePath, "r");
    if (filePtr == NULL) {
        status = FILE_OPENING_ERROR;
    }
    // determine the size of file
    if (status == SUCCESS) {
        fseek(filePtr, 0L, SEEK_END);
        fileSize = ftell(filePtr);
        rewind(filePtr);
    }
    if ((status == SUCCESS) && (fileSize != 0)) {
        str = (char*) malloc((fileSize + 1) * sizeof(char));
        if (str == NULL){
            status = NULL_POINTER_ERROR;
        }
    }
    // copy file content into str
    if (status == SUCCESS) {
        char ch = EOF;
        size_t index = 0;
        while ((ch = getc(filePtr)) != EOF) {
            str[index++] = ch;
        }
        //add end of string char
        str[index] = '\0';
    }
    fclose(filePtr);
    if (status == SUCCESS) {
        status = createQueue(&queue,str, fileSize);
    }
    /* create the huffman encoding tree */
    if (status == SUCCESS) {
        status = createHuffmanTree(tree, &queue);
    }
    logLeave(fName);
    return status;
}

errorId_t createVector(vector* vect, mapNode* count[256]){
    const char fName[] = "createVector";
    errorId_t err = SUCCESS;
    vectorNode* currentNode = NULL;
    size_t index = 0;
    *vect = NULL;
    logEnter(fName);
    while ((err == SUCCESS) && (index < 256)){
        if (count[index] != NULL) {
            if(*vect == NULL) {
                *vect = (vectorNode*) malloc(sizeof(vectorNode));
                if((*vect) == NULL) {
                    err = NULL_POINTER_ERROR;
                }
                if(err == SUCCESS) {
                    (*vect)->val = count[index];
                    (*vect)->next = NULL;
                    currentNode = *vect;
                }
            } else {
                vectorNode* newNode = (vectorNode*) malloc(sizeof(vectorNode));
                if (newNode == NULL) {
                    err = NULL_POINTER_ERROR;
                }
                if( err == SUCCESS) {
                    newNode->next = NULL;
                    newNode->val = count[index];
                    currentNode->next = newNode;
                    currentNode = newNode;
                }
            }
        }
        index++;
    }
    logLeave(fName);
    return err;
}

void printVector(vector vect) {
    const char fName[] = "printVector";
    vectorNode* currentNode = vect;
    logEnter(fName);
    while (currentNode != NULL){
        printf("%c: %u ", currentNode->val->key, currentNode->val->val);
        currentNode = currentNode->next;
    }
    logLeave(fName);
}

errorId_t sort(vector* vect){
    const char fName[] = "sort";
    errorId_t err = SUCCESS;
    bool isSorted = false;
    if(vect == NULL) {
        err = NULL_POINTER_ERROR;
        logError(err,"null pointer in %s",fName);
    }
    while ((err == SUCCESS) && (isSorted == false)) {
        isSorted = true;
        if((*vect)->val->val > (*vect)->next->val->val) {
            mapNode* tmpNode = (*vect)->val;
            (*vect)->val=(*vect)->next->val;
            (*vect)->next->val = tmpNode;
        }
        vectorNode* currentNode = *vect;
        while(currentNode->next != NULL) {
            if(currentNode->val->val > currentNode->next->val->val){
                mapNode* tmpNode = currentNode->val;
                currentNode->val=currentNode->next->val;
                currentNode->next->val = tmpNode;
                isSorted = false;
            }
            currentNode = currentNode->next;
        }

    }
    logLeave(fName);
    return err;
}