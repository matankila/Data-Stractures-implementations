/********************************************/
/*											*/
/*					OL 61					*/
/*					VSA 					*/
/*											*/
/********************************************/
#ifndef _OL61_VSA_ 
#define _OL61_VSA_ 
#include <stddef.h> /* size_t */

typedef struct vsa_block_t vlck_t;

/*
* return value - pointer to administrative struct.
* arguments - pointer to user's malloc, malloc'ed size.
* this function hands over malloc'ed space for management.
*/
vlck_t* VsaInit(void *memory, size_t size);

/*
* return value - pointer to allocated value.
* arguments - administrative struct, and the requested
* size (that must be aligned.)
* this function allocates managed memory.
*/
void* VsaAlloc(vlck_t *vsa, size_t size);

/*
* return value - none.
* arguments - pointer to memory space to be freed.
* this function frees managed memory space.
*/
void VsaFree(void *block);

/*
* return value - the biggest available chunck in the managed memory.
* arguments - pointer to administrative struct.
* this function finds the largest free chunck in the given vsa*/
size_t VsaLargestChunckAvailable(vlck_t *vsa);

#endif /* _OL61_VSA_*/