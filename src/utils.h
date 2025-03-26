#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "types.h" 

typedef struct huffmanNode huffmanNode;
typedef struct vectorNode vectorNode;
typedef struct mapNode mapNode;

struct mapNode{
    char key;
    uint32 val;
};

typedef mapNode* map;

/** 
 * create a map of givin string with the occurence of its chars
 * 
 * @param str: the input string
 * @param len: the len of string
 * @return map of occurences
*/
errorId_t creteMap(char* str, size_t len);

struct huffmanNode{
    huffmanNode* leftChild;  
    huffmanNode* rightChild; 
    char ascii;                 
};

typedef huffmanNode* huffmanTree;

/**
 * Creates a Huffman tree from the given input file.
 *
 * @param tree: A pointer to the root of the Huffman tree.
 * @param inputFile: The input file to be used for creating the tree.
 * @return A status indicating whether the creation was successful.
 */
errorId_t createHuffmanTree(huffmanTree* tree, const char* filePath);


struct vectorNode{
    mapNode* val;
    vectorNode* next;
};

typedef vectorNode* vector;
/**
 * a function to create a vector
 * 
 * @param vector : a pointer that holds the vector to create
 * @return A status indicating whether the creation was successful.
 */
errorId_t createVector(vector* vect, mapNode* count[256]);

/**
 * a function to print the content of vector
 * @param vector : the vector to print
 * @return void
 */
void printVector(vector vect);

/**
 * a function to sort a vector
 * @param vector : a pointer that holds the vector to sort
 * @return A status indicating whether the creation was successful.
 */
errorId_t sort(vector* vect);


#endif // UTILS_H