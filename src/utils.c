#include "utils.h"

map creteMap(char* str, size_t len) {
    long long count[255] = {0};
    for (int i=0; i<len; i++) {
        count[str[i]]++;
    }
    map newMap = NULL;
    errorId_t status = SUCCESS;
    size_t index = 0;
    while ((index < len) && (status == SUCCESS)){
        
    }

}

errorId_t createHuffmanTree(huffmanTree* tree, const char* filePath){
    errorId_t status = SUCCESS;
    FILE *filePtr;
    filePtr = fopen(filePath, "r");
    puts(filePath);
    if (filePtr == NULL) {
        puts("null");
        status = FILE_OPENING_ERROR;
    }
    return status;

}