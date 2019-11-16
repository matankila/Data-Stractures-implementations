#ifndef OL61_QUICK_SORT
#define OL61_QUICK_SORT

#include <stddef.h> /* size_t */

/*
* Arguments: unsorted array, and its size.
* Returned value: none, the array now sorted.
* Time complexity: O(n(log(n))) 
*/
void QuickSort(int *arr, size_t size);

#endif /* OL61_QUICK_SORT */