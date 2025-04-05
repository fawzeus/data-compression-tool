#include "utils.h"
#include "periorityqueue.h"
errorId_t creteMap(char* str, size_t len) {
    errorId_t err = SUCCESS;
    mapNode* count[256] = {NULL};
    periorityQueue queue = NULL;
    for (size_t i = 0; i < len; i++) {
        if (count[(size_t) str[i]] == NULL){
            count[(size_t) str[i]] = (mapNode*) malloc(sizeof(mapNode));
            count[(size_t) str[i]]->key = str[i];
            count[(size_t) str[i]]->val = 1;
        }
        else {
            count[(size_t) str[i]]->val++;
        }
    }/*
    for(int i=0; i<256; i++){
        if (count[i] != NULL){
            printf("%c: %u ",count[i]->key,count[i]->val);
        }
    }*/
    puts("");
    createPeriorityQueue(&queue, count);
    for (size_t i = 0; i < len; i++){
        if(count[i] != NULL) {
            free(count[i]);
        }
    }
    //createVector(&vect, count);
    //printVector(vect);
    print(queue);
    //sort(&vect);
    //printVector(vect);
    err = freeQueue(&queue);
    return err;
#if 0
    map newMap = NULL;
    errorId_t status = SUCCESS;
    size_t index = 0;
    while ((index < len) && (status == SUCCESS)){
        
    }
#endif
}

errorId_t createHuffmanTree(huffmanTree* tree, const char* filePath){
    errorId_t status = SUCCESS;
    FILE *filePtr = NULL;
    size_t fileSize = 0;
    char* str = NULL;
    tree = (huffmanTree*) (sizeof(huffmanTree));
    if (tree != NULL) {
        //TODO
    }
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
    if (status == SUCCESS) {
        char ch = EOF;
        size_t index = 0;
        while ((ch = getc(filePtr)) != EOF) {
            str[index++] = ch;
        }
        //add end of string char
        str[index] = '\0';
    }
    if (status == SUCCESS) {
        status = creteMap(str, fileSize);
    }
    return status;
}

errorId_t createVector(vector* vect, mapNode* count[256]){
    puts("create vect");
    errorId_t err = SUCCESS;
    vectorNode* currentNode = NULL;
    size_t index = 0;
    *vect = NULL;
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
    return err;
}

void printVector(vector vect) {
    puts("print vector");
    vectorNode* currentNode = vect;
    while (currentNode != NULL){
        printf("%c: %u ", currentNode->val->key, currentNode->val->val);
        currentNode = currentNode->next;
    }
    puts("");
}

errorId_t sort(vector* vect){
    errorId_t err = SUCCESS;
    bool isSorted = false;
    if(vect == NULL) {
        err = NULL_POINTER_ERROR;
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
    return err;
    
}