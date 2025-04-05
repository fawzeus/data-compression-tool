#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef uint8_t uint8;
typedef uint32_t uint32;

typedef enum {
    SUCCESS = 0,
    FILE_OPENING_ERROR = 1,
    NULL_POINTER_ERROR = 2,
    NOT_IMPLEMENTED = 3
} errorId_t;


/* periority Queue */
typedef struct queueNode queueNode;
typedef queueNode* periorityQueue;
struct queueNode{
    char key;
    uint32 val;
    queueNode* next;
};

/* boolean */
typedef enum {
    false = 0,
    true = 1
} bool;

typedef struct huffmanNode huffmanNode;
typedef struct vectorNode vectorNode;
typedef struct mapNode mapNode;

struct mapNode{
    char key;
    uint32 val;
};

typedef mapNode* map;


struct vectorNode{
    mapNode* val;
    vectorNode* next;
};

typedef vectorNode* vector;


struct huffmanNode{
    huffmanNode* leftChild;  
    huffmanNode* rightChild; 
    char ascii;                 
};

typedef huffmanNode* huffmanTree;

#endif