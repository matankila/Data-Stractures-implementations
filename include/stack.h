/************************************/
/*    DS worksheet / matan keler    */
/*  Stack worksheet, header file 	*/
/*  		Stack APIs				*/ 
/************************************/

#ifndef _STACK /* preproccessor will not copy it again if its already set */
#define _STACK

#include <stddef.h> /* for size_t */

typedef struct Stack_t Stack_t;

/* create stack in size of (elements * number of elements) and return its pointer  */
Stack_t *StackCreate(size_t size_of_element, size_t number_of_elements);

/* push new element to our stack */
int StackPush(Stack_t *stack_ptr, const void *element_to_add);

/* pop the element at the top of the stack, and return pointer to the element 																	 before  */
void StackPop(Stack_t *stack_ptr);

/* return the current size of the stack */
size_t StackSize(Stack_t *stack_ptr);

/* free stack memory */
void StackDestroy(Stack_t *stack_ptr);

/* return the value at the top without changing the size */
void *StackPeek(Stack_t *stack_ptr);

void StackSortInsert(int data, Stack_t *stack);

void StackSort(Stack_t *stack);

#endif