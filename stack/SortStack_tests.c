#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stack.h"

#define SIZE 20

void SortStackTest();
void SortStackArrTest();
void SortStackRandArrTest();

int main()
{
	SortStackTest();
	SortStackArrTest();
	SortStackRandArrTest();

	return 0;
}

void SortStackTest()
{
	int err_count = 0, err = 0;
	int n1 = 3, n2 = 8, n3 = 14, n4 = 27, n5 = 9;
	
	Stack_t * stack = StackCreate(sizeof(int), 5);

	StackPush(stack, &n2);
	StackPush(stack, &n1); 
	StackPush(stack, &n3);
	StackPush(stack, &n4);
	StackPush(stack, &n5);

	StackSort(stack);

	err_count += (n4 != *(int *)StackPeek(stack));
	if (err_count)
	{
		printf("\33[1;31mError in line %d\33[0;0m\n", __LINE__);
		err_count = 0;
		++err;
	}
	StackPop(stack);

	err_count += (n3 != *(int *)StackPeek(stack));
	if (err_count)
	{
		printf("\33[1;31mError in line %d\33[0;0m\n", __LINE__);
		err_count = 0;
		++err;
	}
	StackPop(stack);

	err_count += (n5 != *(int *)StackPeek(stack));
	if (err_count)
	{
		printf("\33[1;31mError in line %d\33[0;0m\n", __LINE__);
		err_count = 0;
		++err;
	}
	StackPop(stack);

	err_count += (n2 != *(int *)StackPeek(stack));
	if (err_count)
	{
		printf("\33[1;31mError in line %d\33[0;0m\n", __LINE__);
		err_count = 0;
		++err;
	}
	StackPop(stack);

	err_count += (n1 != *(int *)StackPeek(stack));
	if (err_count)
	{
		printf("\33[1;31mError in line %d\33[0;0m\n", __LINE__);
		err_count = 0;
		++err;
	}
	StackPop(stack);

	if (!err)
	{
		printf("\n\33[1;32mAll tests were successful!\33[0;0m\n");
	}

	StackDestroy(stack);
	stack = NULL;
}

void SortStackArrTest()
{
	int i = 0;
	int err_count = 0, err = 0;
	int arr[SIZE] = {0};
	
	Stack_t * stack = StackCreate(sizeof(int), SIZE);

	for (i = 0; i < SIZE; ++i)
	{
		arr[i] = i;
	}

	for (i = SIZE - 1; i > 0; --i)
	{
		StackPush(stack, &arr[i]);
	}

	StackSort(stack);

	for (i = SIZE - 1; i > 0; --i)
	{
		err_count += (arr[i] != *(int *)StackPeek(stack));
		if (err_count)
		{
			printf("\33[1;31mError in line %d, for i = %d\33[0;0m\n", __LINE__, i);
			err_count = 0;
			++err;
		}
		StackPop(stack);
	}

	if (!err)
	{
		printf("\n\33[1;32mAll tests were successful!\33[0;0m\n");
	}

	StackDestroy(stack);
	stack = NULL;
}

void SortStackRandArrTest()
{
	int i = 0, prev_data = 0;
	int err_count = 0, err = 0;
	int arr[SIZE] = {0};
	
	Stack_t * stack = StackCreate(sizeof(int), SIZE);

	srand(time(0));

	for (i = 0; i < SIZE; ++i)
	{
		arr[i] = rand() % (5 * SIZE);
		StackPush(stack, &arr[i]);
	}

	StackSort(stack);

	for (i = SIZE - 1; i > 1; --i)
	{	
		prev_data = *(int *)StackPeek(stack);
		err_count += (prev_data < *(int *)StackPeek(stack));
		if (err_count)
		{
			printf("%ld\n", StackSize(stack));
			printf("\33[1;31mError in line %d, for i = %d\33[0;0m\n", __LINE__, i);
			err_count = 0;
			++err;
		}
		StackPop(stack);
	}

	if (!err)
	{
		printf("\n\33[1;32mAll tests were successful!\33[0;0m\n");
	}

	StackDestroy(stack);
	stack = NULL;
}