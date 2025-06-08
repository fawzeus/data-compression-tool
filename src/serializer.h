#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdio.h>
#include "types.h"

/**
 * @brief Serialize a Huffman tree into a TLV-formatted byte buffer.
 *
 * @param tree: Huffman tree to serialize.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t serializeTree(huffmanTree tree);

/**
 * @brief Recursively serialize a Huffman tree node into TLV format.
 *
 * @param node: Pointer to the current Huffman tree node.
 * @param bb: Pointer to a byte buffer to append the TLV data.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t tlvSerialize(huffmanNode* node, byteBuffer* bb);

/**
 * @brief Create a byte buffer with a given initial size.
 *
 * @param bb: Pointer to the byte buffer structure to initialize.
 * @param bufferSize: Size in bytes to allocate for the buffer.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t byteBufferCreate(byteBuffer* bb, uint32 bufferSize);

/**
 * @brief Free the memory associated with a byte buffer.
 *
 * @param bb: Pointer to the byte buffer to destroy.
 */
void byteBufferDestroy(byteBuffer* bb);

/**
 * @brief Append a TLV-encoded entry to the byte buffer.
 *
 * @param bb: Pointer to the byte buffer.
 * @param tag: Pointer to the tag value (e.g., node type or field identifier).
 * @param tagVal: Pointer to the value associated with the tag.
 * @param tagLen: Length of the value.
 */
void tlvBufferAppend(byteBuffer* bb, const uint8_t* tag, const uint8_t* tagVal, uint16_t tagLen);

/**
 * @brief Write a given integer value into a byte buffer of a specific size.
 *
 * The integer is written in big-endian format.
 *
 * @param input_int: Integer value to convert.
 * @param output_buffer: Buffer where the converted bytes will be written.
 * @param size: Number of bytes to use (e.g., 2 for uint16, 4 for uint32).
 */
void convertIntIntoByteBuffer(uint32 input_int, uint8* output_buffer, size_t size);

/**
 * @brief Deserialize a TLV buffer and create a priority queue of Huffman nodes.
 *
 * @param bb: Byte buffer containing the TLV-encoded Huffman tree.
 * @param queue: Pointer to the priority queue to fill.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t createHuffmanQueueFromTlv(byteBuffer* bb, periorityQueue* queue);

/**
 * @brief Parse a TLV-encoded byte buffer into a priority queue.
 *
 * @param bb: Byte buffer containing TLV-encoded data.
 * @param queue: Output priority queue of Huffman nodes.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t parseTlvTreeData(byteBuffer* bb, periorityQueue* queue);

/**
 * @brief Retrieve the value of a specific tag from a TLV buffer.
 *
 * @param buffer: Byte array containing TLV data.
 * @param bufferLen: Length of the TLV buffer.
 * @param tag: Tag to search for.
 * @param tagLen: Length of the tag.
 * @param val: Output pointer to the value found (within buffer).
 * @param valLen: Output value length.
 */
void tlvGetTagValue(uint8* buffer, size_t bufferLen, uint8* tag, size_t tagLen, uint8** val, size_t* valLen);

/**
 * @brief Decode a single TLV entry from a buffer.
 *
 * @param bufferArray: Pointer to the current buffer pointer (will be advanced).
 * @param tag: Output pointer to the decoded tag.
 * @param tagLen: Output tag length.
 * @param val: Output pointer to the decoded value.
 * @param valLen: Output value length.
 * @param bufferSize: Input/output size of remaining buffer.
 */
void tlvDecode(uint8** bufferArray, uint8** tag, size_t* tagLen, uint8** val, size_t* valLen, size_t* bufferSize);

/**
 * @brief Convert a byte buffer containing an encoded integer to uint32.
 *
 * @param val: Buffer with encoded integer value.
 * @param valLen: Number of bytes to read.
 * @param outputValue: Output integer value.
 */
void convertBufferIntoUint16(uint8* val, size_t valLen, uint32* outputValue);

/**
 * @brief Build a Huffman tree from a buffer containing TLV-encoded nodes.
 *
 * @param tree: Output Huffman tree root.
 * @param queue: Priority queue previously built from TLV data.
 * @return errorId_t: Status code indicating success or error.
 */
errorId_t createHuffmanTreeFromStoredBuffer(huffmanTree* tree, periorityQueue* queue);

#endif // SERIALIZER_H