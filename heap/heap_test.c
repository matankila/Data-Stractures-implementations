#include <stdio.h>  /* IO fucntion */ 
#include <stdlib.h> /* allocation  */
#include <string.h>
#include "heap.h"   /* my header   */

#define BIG_SIZE 1000
#define DUMMY_LEN 8
#define FAIL 1
#define SUCCESS 0
#define PRINT_ERR(string, line, err) printf("%s %d\n", string, line); \
                                     ++err;
#define CHECK_PUSH(heap, num) 	if (1 == HeapPush(heap, &num)) \
	                            { \
		                           PRINT_ERR("push failed!", __LINE__, err); \
	                            } 
#define CHECK_PEEK(heap, value) if (value != *(int *)HeapPeek(heap)) \
	                            { \
		                           PRINT_ERR("peek failed!", __LINE__, err); \
	                            } 
#define CHECK_REMOVE(heap, num, IsM) \
	                            if (1 == HeapRemove(heap, &num ,IsM, NULL)) \
                                { \
    	                           PRINT_ERR("remove failed", __LINE__, err); \
                                }
#define CHECK_SIZE(heap, size) 	if (size != HeapSize(heap)) \
	                            { \
		                           PRINT_ERR("size failed!", __LINE__, err); \
	                            }
#define CHECK_INIT(heap) 	    if (NULL == heap) \
	                            { \
		                           PRINT_ERR("init failed!", __LINE__, err); \
	                            }
#define CHECK_EMPTY(heap) 	    if (1 == HeapIsEmpty(heap)) \
	                            { \
		                          PRINT_ERR("isempty failed", __LINE__, err); \
	                            }
#define CHECK_NOT_EMPTY(heap) 	if (0 == HeapIsEmpty(heap)) \
	                            { \
		                          PRINT_ERR("isempty failed", __LINE__, err); \
	                            }
int err = 0;
int test = 0;

int Comp(const void *internal_data,
				const void *external_data,
				void *param);
unsigned int IsM(void *const data, void *const param);
void TestSizeAndIsemptyOnNewHeap();
void TestSizeAndIsEmptyOnFictiveHeap();
void TestSizeAndIsemptyAfterPush();
void TestSizeAndIsemptyAfterPop();
void TestSizeAndIsEmptyAfterRemove();
void StresTst();

int main()
{
	TestSizeAndIsemptyOnNewHeap();
	TestSizeAndIsEmptyOnFictiveHeap();
	TestSizeAndIsemptyAfterPush();
	TestSizeAndIsemptyAfterPop();
	TestSizeAndIsEmptyAfterRemove();
	StresTst();

	return (SUCCESS);
}

int Comp(const void *internal_data, const void *external_data, void *param)
{
	(void)param;

	return (*(int *)external_data - *(int *)internal_data);
}

unsigned int IsM(void *const data, void *const param)
{
	return (*(int *)data == *(int *)(((ret_data_t *)param)->data));
}

void TestSizeAndIsemptyOnNewHeap()
{
	int **arr_of_pointers = NULL;
	heap_t *heap = NULL;
	size_t size = 0;

	arr_of_pointers = malloc(sizeof(int *) * DUMMY_LEN);
	if (NULL == arr_of_pointers)
	{
		PRINT_ERR("malloc failed!", __LINE__, err);

		return ;
	}
	memset(arr_of_pointers, 0, sizeof(int *) * DUMMY_LEN);

	heap = malloc(HeapGetSizeToAlloc());

	heap = HeapInit(heap,(void **)arr_of_pointers, DUMMY_LEN, Comp, NULL);
	CHECK_INIT(heap);

	if (0 == HeapIsEmpty(heap))
	{
		PRINT_ERR("isempty failed!", __LINE__, err);
	}

	CHECK_SIZE(heap, size);

	free(heap);
	free(arr_of_pointers);
	heap = NULL;
	arr_of_pointers = NULL;
}

void TestSizeAndIsEmptyOnFictiveHeap()
{
	int num1 = 11;
	int num2 = 3;
	int num3 = 2;
	int **arr_of_pointers = NULL;
	heap_t *heap = NULL;
	size_t size = 3;

	arr_of_pointers = malloc(sizeof(int *) * DUMMY_LEN);
	if (NULL == arr_of_pointers)
	{
		PRINT_ERR("malloc failed!", __LINE__, err);

		return ;
	}
	memset(arr_of_pointers, 0, sizeof(int *) * DUMMY_LEN);
	arr_of_pointers[0] = &num1;
	arr_of_pointers[1] = &num2;
	arr_of_pointers[2] = &num3;

	heap = malloc(HeapGetSizeToAlloc());

	heap = HeapInit(heap,(void **)arr_of_pointers, DUMMY_LEN, Comp, NULL);
	CHECK_INIT(heap);

	if (1 == HeapIsEmpty(heap))
	{
		PRINT_ERR("isempty failed!", __LINE__, err);
	}

	CHECK_SIZE(heap, size);

	free(heap);
	free(arr_of_pointers);
	heap = NULL;
	arr_of_pointers = NULL;
}

void TestSizeAndIsemptyAfterPush()
{
	int num1 = 11;
	int num2 = 20;
	size_t size = 2;
	int **arr_of_pointers = NULL;
	heap_t *heap = NULL;

	arr_of_pointers = malloc(sizeof(int *) * DUMMY_LEN);
	if (NULL == arr_of_pointers)
	{
		PRINT_ERR("malloc failed!", __LINE__, err);

		return ;
	}
	memset(arr_of_pointers, 0, sizeof(int *) * DUMMY_LEN);

	heap = malloc(HeapGetSizeToAlloc());

	heap = HeapInit(heap,(void **)arr_of_pointers, DUMMY_LEN, Comp, NULL);
	CHECK_INIT(heap);
	CHECK_PUSH(heap, num1);
	CHECK_PUSH(heap, num2);
	CHECK_EMPTY(heap);
	CHECK_SIZE(heap, size);    
	CHECK_PEEK(heap, 20);

	free(heap);
	free(arr_of_pointers);
	heap = NULL;
	arr_of_pointers = NULL;
}

void TestSizeAndIsemptyAfterPop()
{
	int num1 = 11;
	int num2 = 3;
	int num3 = 2;
	size_t size = 3;
	int **arr_of_pointers = NULL;
	heap_t *heap = NULL;

	arr_of_pointers = malloc(sizeof(int *) * DUMMY_LEN);
	if (NULL == arr_of_pointers)
	{
		PRINT_ERR("malloc failed!", __LINE__, err);

		return ;
	}
	memset(arr_of_pointers, 0, sizeof(int *) * DUMMY_LEN);
	arr_of_pointers[0] = &num1;
	arr_of_pointers[1] = &num2;
	arr_of_pointers[2] = &num3;
	heap = malloc(HeapGetSizeToAlloc());

	heap = HeapInit(heap,(void **)arr_of_pointers, DUMMY_LEN, Comp, NULL);
	CHECK_INIT(heap);

	CHECK_EMPTY(heap);
	CHECK_SIZE(heap, size);
	HeapPop(heap);
	--size;

	CHECK_EMPTY(heap);
	CHECK_SIZE(heap, size);
	CHECK_PEEK(heap, 3);
	HeapPop(heap);
	--size;

	CHECK_EMPTY(heap);
	CHECK_SIZE(heap, size);
	CHECK_PEEK(heap, 2);
	HeapPop(heap);
	--size;

	CHECK_NOT_EMPTY(heap);
	CHECK_SIZE(heap, size);

	free(heap);
	free(arr_of_pointers);
	heap = NULL;
	arr_of_pointers = NULL;	
}

void TestSizeAndIsEmptyAfterRemove()
{
	int num1 = 11;
	int num2 = 3;
	int num3 = 2;
	int num4 = 9;
	int num5 = 4;
	int num6 = 1;
	size_t size = 3;
	int **arr_of_pointers = NULL;
	heap_t *heap = NULL;

	arr_of_pointers = malloc(sizeof(int *) * DUMMY_LEN);
	if (NULL == arr_of_pointers)
	{
		PRINT_ERR("malloc failed!", __LINE__, err);

		return ;
	}
	memset(arr_of_pointers, 0, sizeof(int *) * DUMMY_LEN);
	heap = malloc(HeapGetSizeToAlloc());

	heap = HeapInit(heap,(void **)arr_of_pointers, DUMMY_LEN, Comp, NULL);
	CHECK_INIT(heap);
	CHECK_PUSH(heap, num1);
	CHECK_PUSH(heap, num2);
	CHECK_PUSH(heap, num3);

	CHECK_EMPTY(heap);
	CHECK_SIZE(heap, size);
	++size;

	CHECK_PUSH(heap, num4);
	CHECK_SIZE(heap, size);
	++size;
	CHECK_PEEK(heap, 11);

    CHECK_PUSH(heap, num5);
	CHECK_SIZE(heap, size);
	++size;
	CHECK_PEEK(heap, 11);

    CHECK_PUSH(heap, num6);
    CHECK_SIZE(heap, size);
    CHECK_PEEK(heap, 11);

	CHECK_REMOVE(heap, num1, IsM);
	--size;
	CHECK_SIZE(heap, size);
	CHECK_PEEK(heap, 9);

	CHECK_REMOVE(heap, num6, IsM);
	--size;
	CHECK_SIZE(heap, size);	
    CHECK_PEEK(heap, 9);

    free(heap);
	free(arr_of_pointers);
	heap = NULL;
	arr_of_pointers = NULL;	
}

void StresTst()
{
	heap_t *heap = NULL;
	int arr[BIG_SIZE] = {0};
	int **array = NULL;
	size_t i = 0;

	array = malloc(sizeof(int *) * BIG_SIZE);
	if (NULL == array)
	{
		PRINT_ERR("malloc failed!", __LINE__, err);

		return ;
	}
	memset(array, 0, sizeof(int *) * BIG_SIZE);

	for (i = 0; i < BIG_SIZE; ++i)
	{
		arr[i] = i + 1;

		if (57 == i)
		{
			arr[i] = -23;
		}
	}

	heap = malloc(HeapGetSizeToAlloc());
	heap = HeapInit(heap,(void **)array, BIG_SIZE, Comp, NULL);
	CHECK_INIT(heap);

	for (i = 0; i < BIG_SIZE; ++i)
	{
    	CHECK_PUSH(heap, arr[i]);
    	CHECK_SIZE(heap, (i + 1));
    	if (i == 57)
    	{
    		CHECK_PEEK(heap, arr[i - 1]);
    		continue;
    	}
		CHECK_PEEK(heap, arr[i]);
	}

	for (i = BIG_SIZE - 1; i > 1; --i)
	{
    	HeapPop(heap);
    	CHECK_SIZE(heap, (i));
    }

	free(heap);
	free(array);
	heap = NULL;
	array = NULL;	
}