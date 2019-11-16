#ifndef OL61_DYNAMIC_VECTOR
#define OL61_DYNAMIC_VECTOR

#include <stddef.h> /* for size_t */

typedef struct dv_t dv_t;

/*creates the vector struct and implements the size of element 
and the number of elements*/
dv_t* DvCreate(size_t size_of_element, size_t num_of_elements);

/*push an element to the top of the vector. returns 0 for success,
if a realloc is called, returns new capacity, and -1 if error accured*/
int DvPushBack(dv_t* dv, const void* element);

/*pop an element to the top of the vector. returns 0 for success,
if a realloc is called, return new (smaller) capacity, and -1 for error*/
int DvPopBack(dv_t* dv);

/* do not reserve a newcapacity smaller then current size */
int DvReserve(dv_t* dv, size_t new_capacity); 

/* if used push/ pop/ reserve after, do not use the returned pointer */
void* DvGetItemByIndex(dv_t* dv, size_t index_of_element); 

/* operates in o(1) */
size_t DvSize(const dv_t* dv);

size_t DvCapacity(const dv_t* dv);

/*free the memory of the vector*/
void DvDestroy(dv_t* dv);

#endif