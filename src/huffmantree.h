#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include "types.h"
/**
 * Creates a Huffman tree from the given count queue.
 *
 * @param tree: A pointer to the root of the Huffman tree.
 * @param queue: queue that contain counts of every caracter.
 * @return A status indicating whether the creation was successful.
 */
errorId_t createHuffmanTree(huffmanTree* tree, periorityQueue* queue);
#endif
