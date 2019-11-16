#include <stdio.h>  /* IO fucntion     */
#include <assert.h> /* assert          */
#include <stdlib.h> /* malloc          */
#include "stack.h"  /* my stack header */
#include <string.h> /* memcpy          */

struct Stack_t
{
	size_t size_of_element;
	void *end_of_stack;
	void *top_p;
	void *base_p;
};

/* create stack in size of (elements * number of elements) and return its pointer  */
Stack_t *StackCreate(size_t size_of_element, size_t number_of_elements)
{
	Stack_t *ptr_to_stack = malloc(sizeof(Stack_t) + (size_of_element * 							   (number_of_elements + 1)));
	if(!ptr_to_stack)
	{
		return NULL;
	}

	ptr_to_stack->size_of_element = size_of_element;
	ptr_to_stack->base_p = (char*)ptr_to_stack + sizeof(Stack_t);
	ptr_to_stack->top_p = (char*)ptr_to_stack->base_p;
	ptr_to_stack->end_of_stack = (char*)(ptr_to_stack->base_p) + (size_of_element * number_of_elements);

	return ptr_to_stack;
}

/* push new element to our stack */
int StackPush(Stack_t *stack_ptr, const void *element_to_add)
{
	assert(stack_ptr != NULL);

	if(stack_ptr->top_p >= stack_ptr->end_of_stack)
	{
		return 1;
	}

	stack_ptr->top_p = (char*)stack_ptr->top_p + stack_ptr->size_of_element;
	memcpy(stack_ptr->top_p, element_to_add, stack_ptr->size_of_element);

	return 0;
}

/* pop the element at the top of the stack, and return pointer to the element 																	 before  */
void StackPop(Stack_t *stack_ptr)
{
	assert(stack_ptr != NULL);

	stack_ptr->top_p = (char*)stack_ptr->top_p - stack_ptr->size_of_element;
}

/* return the current size of the stack */
size_t StackSize(Stack_t *stack_ptr)
{
	assert(stack_ptr != NULL);

	return ((char*)stack_ptr->top_p - (char*)stack_ptr->base_p) / (size_t)(stack_ptr->size_of_element);
}

/* free stack memory */
void StackDestroy(Stack_t *stack_ptr)
{
	assert(stack_ptr != NULL);
	
	free(stack_ptr);

	stack_ptr = NULL;
}

/* return the value at the top without changing the size */
void *StackPeek(Stack_t *stack_ptr)
{
	return (stack_ptr->top_p);
}

void StackSortInsert(int data, Stack_t *stack)
{
	int status = 0;
	int current_peek = 0;

	assert(stack);

	if ((0 == StackSize(stack)) || (*(int *)StackPeek(stack) < data))
	{
		status = StackPush(stack, &data);
		assert(0 == status);

		return;
	}

	current_peek = *(int *)StackPeek(stack);
	StackPop(stack);
	StackSortInsert(data, stack);
	status = StackPush(stack, &current_peek);
	assert(0 == status);
}

void StackSort(Stack_t *stack)
{
	int current_peek = 0;

	assert(stack);

	if (StackSize(stack) == 0)
	{
		return ;
	}

	current_peek = *(int *)StackPeek(stack);
	StackPop(stack);
	StackSort(stack);
	StackSortInsert(current_peek, stack);
}