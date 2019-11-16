#ifndef OL61_BINARY_SEARCH
#define OL61_BINARY_SEARCH

#include <stddef.h> /* size_t */

/* Binary Search
 * Arguments: sorted array of integers, its length, and the num to find
 * Return value: if found, itll return pointer to the number in the array
   otherwise itll return NULL. 
*/
int *BinarySearch(const int *arr, size_t size, int to_find);

/* Binary Search Recusive
 * Arguments: sorted array of integers, its length, and the num to find
 * Return value: if found, itll return pointer to the number in the array
   otherwise itll return NULL. 
*/
int *BinarySearchRec(const int *arr, size_t size, int to_find);

#endif /* OL61_BINARY_SEARCH */