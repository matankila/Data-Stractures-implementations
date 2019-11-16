#ifndef OL61_CBUFF
#define OL61_CBUFF

#include <stddef.h> /* for size_t */

typedef struct cb_t cb_t; /* struct of circular buffer */

/* create circular buffer and allocate it memory */
cb_t* CbuffCreate(size_t n_bytes);

/* copy from buffer to output var, use of n_bytes bigger than capacity 
											may lead to unspecified behaviour */
size_t CbuffRead(cb_t* cb, void* output, size_t n_bytes);

size_t CbuffWrite(cb_t* cb, const void* input, size_t n_bytes);

size_t CbuffCapacity(const cb_t* cb);

size_t CbuffFreeSpace(const cb_t* cb);

void CbuffDestroy(cb_t* cb);

int CbuffIsBuffEmpty(const cb_t* cb);
#endif