#ifndef OL61_JUMP_SEARCH
#define OL61_JUMP_SEARCH

#include <stddef.h> /* size_t */

/* Jump Search
 * Arguments: sorted array of integers, its length, and the num to find
 * Return value: if found, itll return pointer to the number in the array
   otherwise itll return NULL. 
*/
int *JumpSearch(const int *arr, size_t size, int to_find);

#endif /* OL61_JUMP_SEARCH */