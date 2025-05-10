#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <stdio.h>
#include "types.h"

errorId_t serializeTree(huffmanTree tree);
errorId_t tlvSerialize(huffmanNode* node, byteBuffer* bb);
errorId_t byteBufferCreate(byteBuffer* bb, uint32 bufferSize);
void byteBufferDestroy(byteBuffer* bb);
void tlvBufferAppend(byteBuffer* bb, const uint8_t* tag, const uint8_t* tagVal, uint16_t tagLen);
/**
 * write a given int inside a buffer of a given size;
 *
 * @param input_int: an unsigned int that holds the value to be converted
 * @param output_buffer: the buffer where you wanna write the value of input_int
 * @param size: the number of bytes where u wanna write the given value
 * @return void.
 */
void convertIntIntoByteBuffer(uint32 input_int, uint8* output_buffer, size_t size);
errorId_t deserializeTree(huffmanTree* tree, byteBuffer* bb);
errorId_t parseTlvTreeData(byteBuffer* bb, periorityQueue* queue);
void tlvDecode(uint8** bufferArray, uint8** tag, size_t* tagLen, uint8** val, size_t* valLen, size_t* bufferSize);
#endif