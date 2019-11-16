#include <stdio.h>  /* IO fucntion       */
#include <assert.h> /* assert            */
#include <stdlib.h> /* memory allocation */
#include <string.h> /* memmove           */
#include "cbuff.h"  /* my header         */

#define FREE_SPACE(write, read, capacity) (write > read) ? (capacity - write +read) : (read - write)
#define MIN2(num1, num2) ((num1) < (num2)) ? (num1) : (num2)

static void WriteToBuffer(cb_t** cb, const void** data, size_t *n_bytes, size_t *min, size_t *how_much);
static void CopyFromBuffer(cb_t** cb, void** data, size_t *n_bytes, size_t *min, size_t *how_much);

struct cb_t
{
	size_t capacity;
	size_t read_offset;
	size_t write_offset;
	void* base;
};

/* create circular buffer and allocate it some memory */
cb_t* CbuffCreate(size_t n_bytes)
{
	/* allocate size for the mangment struct*/
	cb_t *cb = malloc(sizeof(cb_t));
	if (!(cb))
	{
		return NULL;
	}

	/* allocate size for the creation of the cb */
	cb->base = malloc(n_bytes + 1);
	if (!(cb->base))
	{
		return NULL;
	} 

	*(char*)&cb->base += 1; 	/* to keep one byte hidden to chack if its full */
	*((char*)cb->base - 1) = 0; /* set it 0 - not full , 1 - full */
	cb->read_offset = 0;
	cb->write_offset = 0;
	cb->capacity = n_bytes;

	return cb;
}

/* copy from buffer to output var, use of n_bytes bigger than capacity 
											may lead to unspecified behaviour*/
size_t CbuffRead(cb_t* cb, void* output, size_t n_bytes)
{
	size_t min_read = 0;
	size_t read = 0;

	assert(cb);

	if (n_bytes > cb->capacity)
	{
		n_bytes = cb->capacity;
	}

	/* copy all bytes to the output, or until getting to write offset */
	while (read != n_bytes && CbuffIsBuffEmpty(cb) != 1) 
	{
	    CopyFromBuffer(&cb, &output, &n_bytes, &min_read, &read);

	    *((char*)cb->base - 1) = 0; /* not full */

	    /* reset read offset */
	    if (cb->read_offset >= cb->capacity)
	    {
			cb->read_offset = 0;
		}
	}

	return read;
}

/* copy from input to buffer, use of n_bytes bigger than capacity 
											may lead to unspecified behaviour*/
size_t CbuffWrite(cb_t* cb, const void* input, size_t n_bytes)
{
	size_t min_to_write = 0;
	size_t written = 0;

	assert(cb);

	if (n_bytes > cb->capacity)
	{
		n_bytes = cb->capacity;
	}

	/* copy all bytes to the buffer, or until getting to read offset */
	while (written != n_bytes && 1 != *((char*)cb->base - 1))
	{
	  	WriteToBuffer(&cb, &input, &n_bytes, &min_to_write, &written);	

	  	/* reset write offset */
        if (cb->write_offset >= cb->capacity)
        {	
			cb->write_offset = 0;
        }

        /* when offsets are met the buffer is full */
		if (cb->read_offset == cb->write_offset)
		{
			*((char*)cb->base - 1) = 1; /* full */
		}
	}

	return written;
}

/* return capacity */
size_t CbuffCapacity(const cb_t* cb)
{
	assert(cb);

	return cb->capacity;
}

/* return free space */
/* if read bigger than write it will return thier sub */
/* otherwise itll return everything else minus thier sub */
size_t CbuffFreeSpace(const cb_t* cb)
{
	assert(cb);

	/* if the buffer is empty return its capacity */
	if (1 == *((char*)cb->base - 1) && cb->read_offset == cb->write_offset)
	{
		return (0);
	}
	/* if write and read offset equal return capacitiy minus one of them */
	else if (cb->read_offset == cb->write_offset && 0 == *((char*)cb->base - 1))
	{
		return (cb->capacity);
	}
	/* else compute free space with FREE SPACE macro */
	else
	{
		return FREE_SPACE(cb->write_offset, cb->read_offset, cb->capacity);
	}
}

/* free allocated memory */
void CbuffDestroy(cb_t* cb)
{
	assert(cb);

	free(((char*)cb->base - 1));
	cb->base = NULL;
	free(cb);
	cb = NULL;
}

/* return if buffer is empty 0 - not empty , 1 - empty */
int CbuffIsBuffEmpty(const cb_t* cb)
{
	assert(cb);

	if(CbuffFreeSpace(cb) == CbuffCapacity(cb))
	{
		return 1;
	}

	return 0;
}

/****************** help fucntions, propiotry to this file *******************/
/* copy data to cb */
static void WriteToBuffer(cb_t** cb, const void** data, size_t *n_bytes, size_t *min, size_t *how_much)
{
	*min = MIN2((((*cb)->capacity) - ((*cb)->read_offset)), (*n_bytes - *how_much));
	*min = MIN2(*min, (*cb)->capacity - (*cb)->write_offset);

	memcpy(((char*)((*cb)->base) + (*cb)->write_offset), (char*)*data + *how_much, *min);

	(*cb)->write_offset += *min;
	*how_much += *min;
}

/* copy cb to data */
static void CopyFromBuffer(cb_t** cb, void** data, size_t *n_bytes, size_t *min, size_t *how_much)
{		
	*min = MIN2((*n_bytes - *how_much), ((*cb)->capacity - (*cb)->read_offset));
	memcpy((char*)*data + *how_much,((char*)((*cb)->base) + (*cb)->read_offset), *min);
	(*cb)->read_offset += *min;
	*how_much += *min;
}