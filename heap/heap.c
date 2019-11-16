#include <assert.h> /* assert    */
#include <stdlib.h> /* alloction */
#include <string.h> /* memcpy    */
#include "heap.h"   /* my header */

#define FIND_DAD(index)(((long)(index) - 1) / 2)
#define GET_LEFT_CHILD(index)((index) * 2 + 1)
#define GET_RIGHT_CHILD(index)((index) * 2 + 2)
#define FIRST_INDEX 0
#define MIN_POSSIBLE_SIZE 0
#define ELEMENT_TO_REMOVE_NOT_FOUND -1

enum messages
{
	FALSE = 0,
	SUCCESS = 0,
	FAIL = 1,
	TRUE = 1,
	HEAP_FULL = 1,
	ALLOCATION_FAILURE = 1
};

struct heap_t
{
	void **arr;
	size_t num_of_elements;
	heap_comp_func_t cmp_func;
	void *param;
};

/********************************* Declerations ******************************/

static int CompareFatherAndSon(heap_t *heap, size_t father_index, 
	                           size_t child_index);
static int Is_Full(heap_t *heap);
static size_t CalcHeapSize(const heap_t *heap);
static size_t InsertAndReturnIndex(heap_t *heap, void *data);
static void HeapifyUp(heap_t *heap, size_t inserted_index);
static void Swap(void **data1, void **data2);
static void MoveLastNodeToFirstNode(heap_t *heap);
static void HeapifyDown(heap_t *heap);
static size_t GetChild(heap_t *heap, size_t parent, size_t size);
static int GetIndexOfNodeToRemove(heap_t *heap, const void *data,
	                              is_match_func_t is_match, const void *param,
	                              size_t size);

/***************************** Non Api Functions *****************************/

static int CompareFatherAndSon(heap_t *heap, size_t father_index, 
	                           size_t child_index)
{
	return (heap->cmp_func(heap->arr[father_index], 
			               heap->arr[child_index],
			               heap->param));
}
/************************************************/
static int Is_Full(heap_t *heap)
{
	return (NULL != heap->arr[heap->num_of_elements - 1]);
}

/************************************************/
static void Swap(void **data1, void **data2)
{
	void *temp = NULL;

	temp = *data1;
	*data1 = *data2;
	*data2 = temp;
}

/************************************************/
static size_t CalcHeapSize(const heap_t *heap)
{
	size_t index = 0;

	assert(heap);
	
	while (index < heap->num_of_elements && NULL != heap->arr[index])
	{
		++index;
	}
	
	return (index);
}

/************************************************/
static size_t InsertAndReturnIndex(heap_t *heap, void *data)
{
	size_t index_to_insert = FIRST_INDEX;

	assert(heap);
	assert(data);
	assert(FALSE == Is_Full(heap));

	index_to_insert = HeapSize(heap);
	heap->arr[index_to_insert] = data;

	return (index_to_insert);
}

/************************************************/
static void HeapifyUp(heap_t *heap, size_t inserted_index)
{
	int cmp_result = 0;
	size_t dad_index = FIRST_INDEX;

	assert(heap);

	dad_index = FIND_DAD(inserted_index);
	cmp_result = CompareFatherAndSon(heap, dad_index, inserted_index);
	while (cmp_result > 0)
	{
		Swap(&(heap->arr[dad_index]), &(heap->arr[inserted_index]));
		inserted_index = dad_index;
		dad_index = FIND_DAD(inserted_index);
		cmp_result = CompareFatherAndSon(heap, dad_index, inserted_index);
	}
}

/************************************************/
static void MoveLastNodeToFirstNode(heap_t *heap)
{
	size_t last_valid_index = 0;

	assert(heap);

	last_valid_index = HeapSize(heap) - 1;
	heap->arr[FIRST_INDEX] = heap->arr[last_valid_index];
	heap->arr[last_valid_index] = NULL;
}

/************************************************/
static size_t GetChild(heap_t *heap, size_t parent, size_t size)
{
	size_t left_child = 0;
	size_t right_child = 0;
	int comparison = 0;

	assert(heap);

	left_child = GET_LEFT_CHILD(parent);
	right_child = GET_RIGHT_CHILD(parent);
	if (right_child > size - 1 || left_child > size - 1)
	{
		return (size - 1);
	}

	if ((NULL != heap->arr[left_child]) && (NULL != heap->arr[right_child]))
	{
		comparison = CompareFatherAndSon(heap, right_child, left_child);

		return ((0 > comparison) ? (right_child) : (left_child));
	}
	
	return ((NULL == heap->arr[left_child]) ? (right_child) : (left_child));
}

/************************************************/
static void HeapifyDown(heap_t *heap)
{
	size_t current_index = 0;
	size_t child_index = 0;
	size_t size = 0;

	assert(heap);

	child_index = GetChild(heap, current_index, size);
	size = HeapSize(heap);

	while ((NULL != heap->arr[child_index]) && (size > child_index) && 
			(0 > CompareFatherAndSon(heap, child_index, current_index)))
	{
		Swap(&(heap->arr[current_index]), &(heap->arr[child_index]));
		current_index = child_index;
		child_index = GetChild(heap, current_index, size);
	}
}

static int GetIndexOfNodeToRemove(heap_t *heap, const void *data,
	                              is_match_func_t is_match, const void *param,
	                              size_t size)
{
	ret_data_t ret = {NULL, NULL};
	int is_mathc_res = FALSE;
	size_t i = 0;

	ret.data = data;
	ret.param = param;
	is_mathc_res = is_match(heap->arr[i], &ret);
	while (1 != is_mathc_res && i < size - 1)
	{
		++i;
		is_mathc_res = is_match(heap->arr[i], &ret);
	}

	return ((is_mathc_res == FALSE) ? (ELEMENT_TO_REMOVE_NOT_FOUND) :
	                                  ((int)i));
}

/******************************* Api Functions *******************************/

/*
* init the managment struct of the heap.
* return value: handle to the heap.
* Arguments: array, its length, compare fucntion and a parameter.
* NOTE: initiallize the array before hand(you may use calloc/ memset/ etc..)
* NOTE: you must use HeapGetSizeToAlloc to get the correct size for the heap!
*/
heap_t *HeapInit(heap_t *heap, void **arr, size_t num_of_elements, 
	             heap_comp_func_t cmp_func, void *param)
{
	assert(heap);
	assert(arr);
	assert(num_of_elements);
	assert(cmp_func);

	heap->arr = arr;
	heap->num_of_elements = num_of_elements;
	heap->cmp_func = cmp_func;
	heap->param = param;

	return (heap);
}

/*
* return the size of bytes to alloc for the heap
* Arguments: none
* Note: you must use it before heap init!
*/
size_t HeapGetSizeToAlloc()
{
	return (sizeof(heap_t));
}

/*
* determine if the heap is empty.
* Arguments: heap.
* Return value: 1 - empty, 0 - not empty.
*/
int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	assert(heap->arr);

	return (NULL == heap->arr[0]);
}

/*
* calculate the heap size.
* Arguments: heap.
* Return value: heap size.
*/
size_t HeapSize(const heap_t *heap)
{
	assert(heap);

	if (NULL == heap || TRUE == HeapIsEmpty(heap))
	{
		return (MIN_POSSIBLE_SIZE);
	}

	return (CalcHeapSize(heap));
}

/*
* push new data to the heap.
* return value: 0 if succeed, 1 if failed (heap is full).
* Arguments: heap , data to insert
* NOTE: when heap full return 1
*/
int HeapPush(heap_t *heap, void *data)
{
	size_t inserted_index = 0;

	assert(heap);
	assert(data);

	if (TRUE == HeapIsEmpty(heap))
	{
		heap->arr[FIRST_INDEX] = data;
	}
	else if (FALSE == Is_Full(heap))
	{
		inserted_index = InsertAndReturnIndex(heap, data);
		HeapifyUp(heap, inserted_index);
	}
	else
	{
		return (HEAP_FULL);
	}

	return (SUCCESS);
}

/*
* peek to the first element on the heap root.
* Arguments: heap.
* Return value: heaps peek value.
*/
void *HeapPeek(heap_t *heap)
{
	assert(heap);
	assert(heap->arr);

	return (heap->arr[0]);
}

/*
* pop the first data from the heap,
* and fix the heap by the new root.
* Arguments: heap.
* Return value: none.
*/
void HeapPop(heap_t *heap)
{
	assert(heap);

	if (1 == HeapSize(heap))
	{
		heap->arr[0] = NULL;
	}
	else if (FALSE == HeapIsEmpty(heap))
	{
		MoveLastNodeToFirstNode(heap);
		HeapifyDown(heap);	
	}
}	

/*
* remove a specific element from the heap.
* Arguments: heap, the data to remove, is match fucntion, and a parameter.
* Return value: 0 - success , 1 - failure.
*/
int HeapRemove(heap_t *heap, const void *data, is_match_func_t is_match,
			   const void *param)
{
	int index_of_element_to_remove = 0;
	size_t size = 0;
	
	assert(heap);
	assert(data);
	assert(is_match);

	if (TRUE == HeapIsEmpty(heap))
	{
		return FAIL;
	}

	size = HeapSize(heap);
	index_of_element_to_remove = GetIndexOfNodeToRemove(heap, data, 
		                                                is_match, param,
		                                                size);
	if (ELEMENT_TO_REMOVE_NOT_FOUND == index_of_element_to_remove)
	{
		return FAIL;
	}

	Swap(&(heap->arr[index_of_element_to_remove]),
		 &(heap->arr[size - 1]));
	heap->arr[size - 1] = NULL;
	HeapifyDown(heap);

	return SUCCESS;
}

/*
* remove a specific element from the heap.
* Arguments: heap, the data to remove, is match fucntion, and a parameter.
* Return value: NULL - on failure.
*/
void *HeapErase(heap_t *heap, const void *data, is_match_func_t is_match,
			   const void *param)
{
	int index_of_element_to_remove = 0;
	void *returned_data = NULL;
	size_t size = 0;
	
	assert(heap);
	assert(data);
	assert(is_match);

	if (TRUE == HeapIsEmpty(heap))
	{
		return NULL;
	}

	size = HeapSize(heap);
	index_of_element_to_remove = GetIndexOfNodeToRemove(heap, data, 
		                                                is_match, param,
		                                                size);
	if (ELEMENT_TO_REMOVE_NOT_FOUND == index_of_element_to_remove)
	{
		return NULL;
	}

	returned_data = heap->arr[index_of_element_to_remove];
	Swap(&(heap->arr[index_of_element_to_remove]),
		 &(heap->arr[size - 1]));
	heap->arr[size - 1] = NULL;
	HeapifyDown(heap);

	return returned_data;
}