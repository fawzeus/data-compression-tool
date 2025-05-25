#ifndef STACK_HPP
#define STACK_HPP

#include "types.h"
#include <stdlib.h>

/**
 * @brief Retrieves the top node of the stack without removing it.
 * 
 * @param stack Stack to peek at.
 * @param node Pointer to a stackNode_t where the top node will be stored.
 * @return SUCCESS if the stack is not empty, an error code otherwise.
 */
errorId_t stackTop(stack_t stack, huffmanNode *node);

/**
 * @brief Returns the number of elements in the stack.
 * 
 * @param stack Stack to check.
 * @param size Pointer to a variable where the size will be stored.
 * @return SUCCESS if the operation succeeds, an error code otherwise.
 */
errorId_t stackSize(stack_t stack, size_t *size);


/**
 * @brief Pushes a new huffmanNode onto the stack.
 * 
 * @param stack Pointer to the stack.
 * @param node Pointer to the huffmanNode to push.
 * @return SUCCESS if the operation succeeds, an error code otherwise.
 */
errorId_t stackAppend(stack_t *stack, huffmanNode* node);

/**
 * @brief Removes the top node from the stack and returns it.
 * 
 * @param stack Pointer to the stack.
 * @param node Pointer to a stackNode_t to store the removed node.
 * @return SUCCESS if the operation succeeds, an error code otherwise.
 */
errorId_t stackPop(stack_t *stack, huffmanNode *node);

/**
 * @brief Checks whether the stack is empty.
 * 
 * @param stack Stack to check.
 * @return true if the stack is empty, false otherwise.
 */
bool isStackEmpty(stack_t stack);

#endif