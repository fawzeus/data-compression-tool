#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "types.h" 

typedef struct {
    char key;
    uint32 val;
    struct mapNode* next;
} mapNode;

typedef mapNode* map;

/** 
 * create a map of givin string with the occurence of its chars
 * 
 * @param str: the input string
 * @param len: the len of string
 * @return map of occurences
*/
errorId_t creteMap(char* str, size_t len);

typedef struct {
    struct huffmanNode* leftChild;  
    struct huffmanNode* rightChild; 
    uint8_t bit;                    
} huffmanNode;

typedef huffmanNode* huffmanTree;

/**
 * Creates a Huffman tree from the given input file.
 *
 * @param tree: A pointer to the root of the Huffman tree.
 * @param inputFile: The input file to be used for creating the tree.
 * @return A status indicating whether the creation was successful.
 */
errorId_t createHuffmanTree(huffmanTree* tree, const char* filePath);


#endif // UTILS_H