#include "serializer.h"
#include "logger.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/************************     defines     ************************/
#define OUTPUT_FILE "out/tree.txt"

#define TLV_BUFFER_MAX_SIZE 4096

#define TREE_ROOT        "\xDF\x01"  // Root Node
#define INTERNAL_NODE    "\xDF\x02"  // Internal Node
#define LEAF_NODE        "\xDF\x03"  // Leaf node
#define ASCII_VALUE      "\xDF\x04"  // ASCII character (for leaves)
#define COUNT_VALUE      "\xDF\x05"  // Frequency count (for any node)

void printTree(huffmanNode* root, int level) {
    if (root == NULL) return;
     // Print current node
    if (root->ascii != NULL) {
        printf("'%c' (%d)\n", *(root->ascii), root->count);
    } else {
        printf("[%d]\n", root->count);
    }
    // Print right subtree
    printTree(root->leftChild, level + 1);

    // Print left subtree
    printTree(root->rightChild, level + 1);
}

errorId_t serializeTree(huffmanTree tree) {
    printTree(tree,0);
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
        fprintf(file,"%02X",bb.array_[i]);
    }
    byteBufferDestroy(&bb);
    fclose(file);
    logLeave(fName);
    return status;
}


errorId_t tlvSerialize(huffmanNode* node, byteBuffer* bb) {
    errorId_t status = SUCCESS;
    const char fName[] = "tlvSerialize";
    uint8 tmpBuffer[2] = {0};
    logEnter(fName);
    if (node !=NULL){
        convertIntIntoByteBuffer(node->count,tmpBuffer, 2);
        if (node->ascii != NULL) {
            byteBuffer wrapper;
            byteBufferCreate(&wrapper, TLV_BUFFER_MAX_SIZE);
            tlvBufferAppend(&wrapper, (uint8*) COUNT_VALUE, tmpBuffer, 2);
            tlvBufferAppend(&wrapper, (uint8*) ASCII_VALUE, (uint8*)node->ascii, 1);
            tlvBufferAppend(bb, (uint8*) LEAF_NODE, wrapper.array_, wrapper.bufferSize);
            byteBufferDestroy(&wrapper);
        }
        else {
            byteBuffer wrapper;
            byteBufferCreate(&wrapper, TLV_BUFFER_MAX_SIZE);
            tlvBufferAppend(&wrapper, (uint8*) COUNT_VALUE, tmpBuffer, 2);
            tlvBufferAppend(bb, (uint8*) INTERNAL_NODE, wrapper.array_, wrapper.bufferSize);
            byteBufferDestroy(&wrapper);
        }
        if(status == SUCCESS ) {
            status = tlvSerialize(node->leftChild, bb);
        }
        if(status == SUCCESS) {
            status = tlvSerialize(node->rightChild, bb);
        }
    } else {
        uint8 tmpBuffer[1] = {0};
        tlvBufferAppend(bb, (uint8*) LEAF_NODE, tmpBuffer, 0);
    }
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
    assert(bb != NULL);
    free(bb->array_);
    bb->array_ = NULL;
    bb->bufferMaxSize = 0;
    bb->bufferSize = 0;
}

void intToHexBytes(uint32_t value, uint8_t* output, size_t width) {
    for (int i = width - 1; i >= 0; --i) {
        uint8_t nibble = (value >> (4 * i)) & 0xF;
        output[width - 1 - i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
    }
}

void tlvBufferAppend(byteBuffer* bb, const uint8_t* tag, const uint8_t* tagVal, uint16_t tagLen) {
    assert(bb != NULL);
    assert(tag != NULL);
    assert(tagVal != NULL);
    // 8 is defined as unsigned here
    if ((bb->bufferSize) < (tagLen + 8U)) {
        uint8* oldBuffer_array = bb->array_;
        bb->bufferMaxSize += tagLen + 8U;
        bb->array_ = (uint8*) malloc(bb->bufferMaxSize *sizeof(uint8));
        memcpy(bb->array_, oldBuffer_array, bb->bufferSize);
        free(oldBuffer_array);
    }

    // Append 2-byte tag
    memcpy(bb->array_ + bb->bufferSize, tag, 2);
    bb->bufferSize += 2;

    // Append 1-byte length (big-endian)
    bb->array_[bb->bufferSize++] = tagLen & 0xFF;

    // Append value
    memcpy(bb->array_ + bb->bufferSize, tagVal, tagLen);
    bb->bufferSize += tagLen;
}

void convertIntIntoByteBuffer(uint32 input_int, uint8* output_buffer, size_t size) {
    assert(output_buffer != NULL);
    for (size_t i = 0; i < size; i++) {
        output_buffer[size - i - 1] = (input_int >> i * 8) & 0xFF;
    }
}

errorId_t deserializeTree(huffmanTree* tree, byteBuffer* bb) {
    errorId_t status = NOT_IMPLEMENTED;
    return status;
}

errorId_t parseTlvTreeData(byteBuffer* bb, periorityQueue* queue) {
    errorId_t status = NOT_IMPLEMENTED;
    return status;
}

void tlvDecode(uint8** bufferArray, uint8** tag, size_t* tagLen, uint8** val, size_t* valLen, size_t* bufferSize) {
    errorId_t status = SUCCESS;
    assert( bufferArray != NULL && *bufferArray != NULL);
    *tag = *bufferArray;
    *tagLen = 2U;
    *bufferArray += 2;
    *bufferSize -= 2;
    *valLen = ((size_t)(*bufferArray)[0] << 8) | (*bufferArray)[1];
    *bufferArray += 2;
    *bufferSize -= 2;
    *val = *bufferArray;
    *bufferArray += *valLen;
    *bufferSize -= *valLen;
}

