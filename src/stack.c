#include "stack.h"
#include <assert.h>
#include "logger.h"
#include <string.h>

errorId_t stackAppend(stack_t *stack, huffmanNode* node) {
    errorId_t status = SUCCESS;
    assert(stack != NULL);
    assert(node != NULL);

    if(stack->head == NULL){
        stack->head = (stackNode_t*) malloc(sizeof(stackNode_t));
        if (stack->head != NULL) {
            stack->head->val = node;
            stack->head->next = NULL;
            stack->size += 1;
        } else {
            status = NULL_POINTER_ERROR;
            logError(status,"error in append, stack.c");
        }
    } else {
        stackNode_t* newNode = (stackNode_t*) malloc(sizeof(stackNode_t));
        newNode->next = stack->head;
        newNode->val = node;
        stack->size += 1;
        stack->head = newNode;
    }
    return status;
}

bool isStackEmpty(stack_t stack) {
    return stack.head == NULL;
}

errorId_t stackSize(stack_t stack, size_t *size) {
   *size = stack.size;
   return SUCCESS;
}

errorId_t stackTop(stack_t stack, huffmanNode *node) {
    assert(node != NULL);
    memcpy(node, stack.head->val, sizeof(huffmanNode));
    return SUCCESS;
}

errorId_t StackPop(stack_t *stack, huffmanNode *node) {
    assert(stack != NULL);
    assert(node != NULL);
    assert(stack->head != NULL);
    assert(stack->size != 0);
    stackNode_t* toBeDeleted = stack->head;
    memcpy(node, toBeDeleted->val, sizeof(huffmanNode));
    stack->head = stack->head->next;
    free (toBeDeleted);
    return SUCCESS;
}
