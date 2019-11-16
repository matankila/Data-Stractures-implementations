/********************************************/
/*											*/
/*					OL 61					*/
/*			Fixed Size Allocator 			*/
/*											*/
/********************************************/

#ifndef OL61_FSA
#define OL61_FSA


#include <stddef.h> /* size_t */

typedef struct fsa_t fsa_t;

/*
* O(n)
* return value - pointer to administrative struct.
* arguments - pointer to user's malloc, malloc'ed size, size of element.
* this function hands over malloc'ed space for management.
*/
fsa_t  *FsaInit(void *memory, size_t size, size_t element_size); 

/*
* O(1)
* return value - calculated size for malloc (includes administratives).
* arguments - element size, number of elements.
* this function calculates the total size that needs to be malloc'ed in order
* for the user to be able to receieve the requested capacity.
*/
size_t  FsaSuggestSize(size_t element_size, size_t element_num); 

/*
* O(1)
* return value - pointer to allocated value.
* arguments - administrative struct.
* this function allocates managed memory.
*/
void   *FsaAlloc(fsa_t *fsa);

/*
* O(1)
* return value - none.
* arguments - pointer to memory space to be freed.
* this function frees managed memory space.
*/
void    FsaFree(void *element);

/*
* O(n)
* return value - number of free elements.
* arguments - administrative struct.
* this function returns number of free elements.
*/
size_t  FsaCountFree(fsa_t *fsa);

#endif /*OL61_FSA*/