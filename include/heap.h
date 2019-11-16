/************/
/*15.01.2019*/
/************/
#ifndef  OL61_HEAP
#define  OL61_HEAP

#include <stddef.h> /*for size_t */

typedef struct heap_t heap_t;
typedef struct ret_data_t ret_data_t;
struct ret_data_t
{
	const void *data;
	const void *param;
};
typedef int (*heap_comp_func_t)(const void *internal_data,
								const void *external_data,
								void *param);
typedef unsigned int (*is_match_func_t)(void *const data, void *const param);

/*
* init the managment struct of the heap.
* return value: handle to the heap.
* Arguments: array, its length, compare fucntion and a parameter.
* NOTE: initiallize the array before hand(you may use calloc/ memset/ etc..)
* NOTE: you must use HeapGetSizeToAlloc to get the correct size for the heap!
*/
heap_t *HeapInit(heap_t *heap, void **arr, size_t num_of_elements,
                 heap_comp_func_t cmp_func, void *param);
/*
* return the size of bytes to alloc for the heap
* Arguments: none
* Note: you must use it before heap init!
*/
size_t HeapGetSizeToAlloc();

/*
* push new data to the heap.
* return value: 0 if succeed, 1 if failed (heap is full).
* Arguments: heap , data to insert
*/
int HeapPush(heap_t *heap, void *data);

/*
* pop the first data from the heap,
* and fix the heap by the new root.
* Arguments: heap.
* Return value: none.
*/
void HeapPop(heap_t *heap);

/*
* peek to the first element on the heap root.
* Arguments: heap.
* Return value: heaps peek value.
*/
void *HeapPeek(heap_t *heap);

/*
* calculate the heap size.
* Arguments: heap.
* Return value: heap size.
*/
size_t HeapSize(const heap_t *heap);

/*
* determine if the heap is empty.
* Arguments: heap.
* Return value: 1 - empty, 0 - not empty.
*/
int HeapIsEmpty(const heap_t *heap);

/*
* remove a specific element from the heap.
* Arguments: heap, the data to remove, is match fucntion, and a parameter.
* Return value: 0 - success , 1 - failure.
*/
int HeapRemove(heap_t *heap, const void *data, is_match_func_t is_match,
			   const void *param);

/*
* remove a specific element from the heap.
* Arguments: heap, the data to remove, is match fucntion, and a parameter.
* Return value: NULL - on failure.
*/
void *HeapErase(heap_t *heap, const void *data, is_match_func_t is_match,
			   const void *param);

#endif /* OL61_HEAP */