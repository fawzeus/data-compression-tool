#include "serializer.h"
#include "logger.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/************************     defines     ************************/
#define OUTPUT_FILE "out/tree.txt"

#define TLV_BUFFER_MAX_SIZE 4096

#define TREE_ROOT    0xDF01  // Root of the tree
#define LEFT_CHILD   0xDF02  // Left child node
#define RIGHT_CHILD  0xDF03  // Right child node
#define ASCII_VALUE  0xDF04  // ASCII character (for leaves)
#define COUNT_VALUE  0xDF05  // Frequency count (for any node)

errorId_t serializeTree(huffmanTree tree) {
    errorId_t status = SUCCESS;
    const char fName[] = "serializeTree";
    byteBuffer bb;
    FILE* file = NULL;
    logEnter(fName);
    if (tree == NULL) {
        status = NULL_POINTER_ERROR;
    }
    if (status == SUCCESS) {
        file = fopen(OUTPUT_FILE, "w");
    }
    if (status == SUCCESS && !file) {
        status = UNABLE_TO_OPEN_FILE;
        perror("Failed to open file");
    }
    if (status == SUCCESS) {
        status = byteBufferCreate(&bb, TLV_BUFFER_MAX_SIZE);
    }
    if (status == SUCCESS) {
        status = tlvSerialize(tree, &bb);
    }
    for (uint32 i=0; i < bb.bufferSize; i++){
        fprintf(file,"%c",bb.array_[i]);
    }
    fclose(file);
    logLeave(fName);
    return status;
}


errorId_t tlvSerialize(huffmanNode* node, byteBuffer* bb) {
    errorId_t status = SUCCESS;
    const char fName[] = "tlvSerialize";
    uint8 tmpBuffer[4];
    logEnter(fName);
    assert(node != NULL);
    convertIntIntoByteBuffer(node->count,tmpBuffer,4);
    tlvBufferAppend(bb, COUNT_VALUE, tmpBuffer, 4);
    if (node->ascii != NULL) {
        tlvBufferAppend(bb, ASCII_VALUE, (uint8*)node->ascii, 1);
    }
    if(status == SUCCESS && node->leftChild != NULL) {
        byteBuffer left;
        status = byteBufferCreate(&left, TLV_BUFFER_MAX_SIZE);
        tlvSerialize(node->leftChild, &left);
        tlvBufferAppend(bb, LEFT_CHILD, left.array_, left.bufferSize);
        byteBufferDestroy(&left);
    }
    if(status == SUCCESS && node->rightChild != NULL) {
        byteBuffer right;
        status = byteBufferCreate(&right, TLV_BUFFER_MAX_SIZE);
        tlvSerialize(node->rightChild, &right);
        tlvBufferAppend(bb, RIGHT_CHILD, right.array_, right.bufferSize);
        byteBufferDestroy(&right);
    }
    logLeave(fName);
    return status;
}

errorId_t byteBufferCreate(byteBuffer* bb, uint32 bufferSize) {
    errorId_t status = SUCCESS;
    bb->bufferMaxSize = bufferSize;
    bb->array_ = (uint8*) malloc(bufferSize*sizeof(uint8));
    bb->bufferSize = 0;
    if (bb->array_ == NULL) {
        status = NULL_POINTER_ERROR;
    }
    return status;
}

void byteBufferDestroy(byteBuffer* bb) {
    free(bb->array_);
    bb->array_ = NULL;
    bb->bufferMaxSize = 0;
    bb->bufferSize = 0;
}

void intToHexStr(uint32 value, char* output, size_t width) {
    assert(output != NULL);
    snprintf(output, width + 1, "%0*X", (int)width, value);
}

void tlvBufferAppend(byteBuffer* bb, uint32 tag, const uint8* tagVal, uint32 tagLen) {
    assert(bb->bufferSize + tagLen + 6 < bb->bufferMaxSize);

    char tagStr[5];      // 4 hex digits + null terminator
    char lenStr[5];      // up to 4 hex digits + null terminator (supporting >255 sizes)
    
    intToHexStr(tag, tagStr, 4);     // e.g., DF05
    intToHexStr(tagLen, lenStr, 4);  // e.g., 0004

    // Append tag as ASCII chars (4 bytes)
    memcpy(bb->array_ + bb->bufferSize, tagStr, 4);
    bb->bufferSize += 4;

    // Append tagLen as ASCII chars (4 bytes)
    memcpy(bb->array_ + bb->bufferSize, lenStr, 4);
    bb->bufferSize += 4;

    // Append tag value (raw bytes, or you can also convert these to hex ASCII if desired)
    memcpy(bb->array_ + bb->bufferSize, tagVal, tagLen);
    bb->bufferSize += tagLen;
}


void convertIntIntoByteBuffer(uint32 input_int, uint8* output_buffer, size_t size) {
    assert(output_buffer != NULL);
    for (size_t i = 0; i < size; i++) {
        output_buffer[size - i - 1] = (input_int >> i * 8) & 0xFF;
    }
}

