#include "utils.h"

errorId_t creteMap(char* str, size_t len) {
    errorId_t err = SUCCESS;
    uint32_t count[256] = {0};
    for (int i=0; i < len; i++) {
        count[(size_t) str[i]]++;
    }
    for(int i=0; i<256; i++){
        if (count[i] != 0){
            printf("%c: %u ",i,count[i]);
        }
    }
    puts("");
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

    filePtr = fopen(filePath, "r");
    if (filePtr == NULL) {
        status = FILE_OPENING_ERROR;
    }
    if (status == SUCCESS) {
        fseek(filePtr, 0L, SEEK_END);
        fileSize = ftell(filePtr);
        rewind(filePtr);
    }
    if (fileSize !=0 && status ==SUCCESS){
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
    }

    if (status == SUCCESS) {
        status = creteMap(str, fileSize);
    }

    return status;

}