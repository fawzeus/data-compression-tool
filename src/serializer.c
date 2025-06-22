#include "serializer.h"
#include "logger.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "periorityqueue.h"
#include "stack.h"
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
    huffmanNode a = {.count = 1, .ascii = "A", .leftChild = NULL, .rightChild = NULL};
    huffmanNode b = {.count = 1, .ascii = "B", .leftChild = NULL, .rightChild = NULL};
    huffmanNode c = {.count = 2, .ascii = "C", .leftChild = NULL, .rightChild = NULL};
    huffmanNode d = {.count = 3, .ascii = "D", .leftChild = NULL, .rightChild = NULL};
    huffmanNode ab = {.count = 2, .ascii = NULL, .leftChild = &a, .rightChild = &b};   // A + B
    huffmanNode ab_c = {.count = 4, .ascii = NULL, .leftChild = &ab, .rightChild = &c}; // (A+B)+C
    huffmanNode root = {.count = 7, .ascii = NULL, .leftChild = &ab_c, .rightChild = &d}; // ((A+B)+C)+D
    huffmanTree testTree = &root;
    errorId_t status = SUCCESS;
    const char fName[] = "serializeTree";
    byteBuffer bb;
    FILE* file = NULL;
    logEnter(fName);
    if (testTree == NULL) {
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
        status = tlvSerialize(testTree, &bb);
    }
    for (uint32 i=0; i < bb.bufferSize; i++){
        fprintf(file,"%02X",bb.array_[i]);
    }
    periorityQueue queue = NULL;
    createHuffmanQueueFromTlv(&bb, &queue);
    uint32 len = 0;
    getQueueSize(queue, &len);
    printf("queue len is %u\n",len);
    queueNode* current = queue;
    while (current != NULL)
    {
        if (current->val == NULL) {
            printf("NULL \n");
        }
        else {
            printf("val : %u ",current->val->count);
            if(current->val->ascii != NULL) {
                printf("ascii :%c ",*current->val->ascii);
            }
            puts("");
        }
        current = current->next;
    }
    puts("");
    
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
        byteBuffer wrapper;
        byteBufferCreate(&wrapper, TLV_BUFFER_MAX_SIZE);
        tlvBufferAppend(&wrapper, (uint8*) COUNT_VALUE, tmpBuffer, 2);
        if (node->ascii != NULL) {
            tlvBufferAppend(&wrapper, (uint8*) ASCII_VALUE, (uint8*)node->ascii, 1);
            tlvBufferAppend(bb, (uint8*) LEAF_NODE, wrapper.array_, wrapper.bufferSize);
        }
        else {
            tlvBufferAppend(bb, (uint8*) INTERNAL_NODE, wrapper.array_, wrapper.bufferSize);
        }
        byteBufferDestroy(&wrapper);
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
static void printHex(const uint8* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", data[i]);
        if (i < len - 1) printf(" ");
    }
}
errorId_t createHuffmanQueueFromTlv(byteBuffer* bb, periorityQueue* queue) {
    errorId_t status = SUCCESS;
    uint8* ptr = bb->array_;
    size_t remainingSize = bb->bufferSize;
    while ((status == SUCCESS) && (ptr != NULL) && (remainingSize > 0)) {
        uint8* tag = NULL;
        uint8* val = NULL;
        size_t tagLen = 0;
        size_t valLen = 0;

        tlvDecode(&ptr, &tag, &tagLen, &val, &valLen, &remainingSize);

        printf("Tag: ");
        printHex(tag, tagLen);
        printf(" | TagLen: %zu | ValLen: %zu | Val: ", tagLen, valLen);
        printHex(val, valLen);
        printf("\n");

        switch (tag[0])
        {
        case 0xDF:
            switch (tag[1])
            {
            case 0x2: {
                uint8* subVal = NULL;
                size_t subValLen = 0;
                if ((val != NULL) && (valLen != 0)) {
                    huffmanNode* node = (huffmanNode*) malloc(sizeof(huffmanNode));
                    tlvGetTagValue(val, valLen, (uint8*) "\xDF\x05", 2, &subVal, &subValLen);
                    convertBufferIntoUint16(subVal, subValLen, &node ->count);
                    node->ascii  = NULL;
                    node->leftChild = NULL;
                    node->rightChild = NULL;
                    push(queue, node, false);
                }
                else {
                    status = NULL_POINTER_ERROR;
                }
                break;
            }
            case 0x03: {
                uint8* subVal = NULL;
                size_t subValLen = 0;
                if ((val != NULL) && (valLen != 0)) {
                    huffmanNode* node = (huffmanNode*) malloc(sizeof(huffmanNode));
                    node->ascii  = NULL;
                    node->leftChild = NULL;
                    node->rightChild = NULL;
                    tlvGetTagValue(val, valLen, (uint8*) "\xDF\x05", 2, &subVal, &subValLen);
                    convertBufferIntoUint16(subVal, subValLen, &node ->count);
                    tlvGetTagValue(val, valLen, (uint8*) "\xDF\x04", 2, &subVal, &subValLen);
                    node->ascii = (char*) malloc(sizeof(char));
                    memcpy(node->ascii, subVal, 1);
                    push(queue, node, false);
                }
                break;
            }
            default:
                status = INVALID_TAG_ERROR;
                break;
            }
            break;
        default:
            status = INVALID_TAG_ERROR;
            break;
        }
    }

    return status;
}

errorId_t parseTlvTreeData(byteBuffer* bb, periorityQueue* queue) {
    errorId_t status = NOT_IMPLEMENTED;
    assert(bb != NULL);
    assert(queue != NULL);
    return status;
}

void tlvDecode(uint8** bufferArray, uint8** tag, size_t* tagLen, uint8** val, size_t* valLen, size_t* bufferSize) {
    assert( bufferArray != NULL && *bufferArray != NULL);
    *tag = *bufferArray;
    *tagLen = 2U;
    *bufferArray += 2;
    *bufferSize -= 2;
    *valLen = (size_t) *bufferArray[0];
    *bufferArray += 1;
    *bufferSize -= 1;
    *val = *bufferArray;
    *bufferArray += *valLen;
    *bufferSize -= *valLen;
}

void tlvGetTagValue( uint8* buffer, size_t bufferLen, uint8* tag, size_t tagLen, uint8** val, size_t* valLen) {
    assert(buffer != NULL);
    assert(tag != NULL);
    assert(tagLen != 0);
    assert(bufferLen != 0);

    char fName[] = "tlvGetTagValue";
    bool isFound = false;
    size_t index = 0;
    logEnter(fName);
    while((isFound == false) && (index < bufferLen)) {
        if(memcmp(buffer + index, tag, tagLen) == 0) {
            *valLen = buffer[index+ tagLen];
            *val = buffer + (index + tagLen + 1);
            isFound = true;
        }
        index += 1;
    }
    logLeave(fName);
}

void convertBufferIntoUint16(uint8* val, size_t valLen, uint32* outputValue) {
    *outputValue = 0;
    for (size_t i = 0; i < valLen; i++) {
        *outputValue += val[i]<< (8*(valLen - i - 1));
    }
}

errorId_t createHuffmanTreeFromStoredBuffer(huffmanTree* tree, periorityQueue* queue) {
    errorId_t status = SUCCESS;
    bool isEmptyQueue = true;
    stack_t stack;
    isEmpty(*queue, &isEmptyQueue);
    assert(isEmptyQueue != true);
    huffmanNode* currentNode = NULL;
    size_t stackLength = 0;
    while ((isEmptyQueue != true) && (status == SUCCESS)) {
        status = pop(queue, &currentNode);
        isEmpty(*queue, &isEmptyQueue);
        stackSize(stack, &stackLength);
        if ((currentNode != NULL) && (currentNode->ascii != NULL)) {
            status = stackAppend(&stack, currentNode);
        } else if ((currentNode != NULL) &&
                   (currentNode->ascii == NULL) &&
                   (isStackEmpty(stack) != true) &&
                   (stackLength >= 2)) {
            huffmanNode* leftNode = NULL;
            huffmanNode* rightNode = NULL;
            stackPop(&stack, leftNode);
            stackPop(&stack, rightNode);
            currentNode->leftChild = leftNode;
            currentNode->rightChild = rightNode;
            status = stackAppend(&stack, currentNode);
        }
    }
    stackSize(stack, &stackLength);
    if ((status == SUCCESS) && (stackLength == 1)) {
        stackPop(&stack, currentNode);
        *tree = currentNode;
    }
    return status;
}