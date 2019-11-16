#include <stdio.h> /* IO fucntion */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc realloc free */
#include "dynamic_vector.h" /* my header */

struct dv_t
{
	size_t size_of_element;
	size_t number_of_element;
	size_t capacity;	
	void *base_p;
};

/*creates the vector struct and implements the size of element 
and the number of elements*/
dv_t* DvCreate(size_t size_of_element, size_t num_of_elements)
{
	/* allocate size for the mangment struct*/
	dv_t *dv_ptr = malloc(sizeof(dv_t));
	if(!(dv_ptr))
	{
		return NULL;
	}

	/* allocate size for the creation of the dv */
	dv_ptr->base_p = malloc(size_of_element * num_of_elements);
	if(!(dv_ptr->base_p))
	{
		return NULL;
	} 

	dv_ptr->size_of_element = size_of_element;
	dv_ptr->number_of_element = 0;
	dv_ptr->capacity = num_of_elements;

	return dv_ptr;
}

/*push an element to the top of the vector. returns 0 for success,
if a realloc is called, returns new capacity, and -1 if error accured*/
int DvPushBack(dv_t* dv, const void* element)
{
	/* assert addresses are not NULL */
	assert(dv->base_p);
	assert(element);

	if(dv->capacity == dv->number_of_element)
	{
		/* reallocate 2 times the original space */
		dv->base_p = realloc(dv->base_p, ((dv->capacity) * 2) * dv->												size_of_element);
		if(!(dv->base_p))
		{
			return 1;
		}

		dv->capacity *= 2; /* update the capacity */
	}

	/* copy element to current pointer */
	memcpy(((char*)(dv->base_p) + ((dv->number_of_element) * 											(dv->size_of_element))),									  element, dv->size_of_element);

	++(dv->number_of_element);

	return 0;
}

/*pop an element to the top of the vector. returns 0 for success,
if a realloc is called, return new (smaller) capacity, and -1 for error*/
int DvPopBack(dv_t* dv)
{
	assert(dv->base_p);

	if(((dv->number_of_element) - 1) <= ((dv->capacity) / 4))
	{
		dv->base_p = realloc(dv->base_p, (((dv->capacity) / 2)) * 											 dv->size_of_element);
		if(!dv->base_p)
		{
			return 1;
		}

		dv->capacity /= 2;
	}

	/* we now dont have the past value of the size */
	--(dv->number_of_element);

	return 0;
}

/* do not reserve a newcapacity smaller then current size */
int DvReserve(dv_t* dv, size_t new_capacity)
{
	assert(dv->base_p);

	/* reallocate 2 times the original space */
	dv->base_p = realloc(dv->base_p, new_capacity);
	if(!(dv->base_p))
	{
		return 1;
	}

	/* update new capacity */
	dv->capacity = new_capacity;

	return 0;
} 

/* if used push/ pop/ reserve after, do not use the returned pointer */
void* DvGetItemByIndex(dv_t* dv, size_t index_of_element)
{
	assert(dv->base_p);
	assert(index_of_element < dv->capacity);

	return ((char*)(dv->base_p) + ((index_of_element) * dv->size_of_element));
} 

/* operates in o(1), get the current size of dv */
size_t DvSize(const dv_t* dv)
{
	assert(dv->base_p);

	return (dv->number_of_element);
}

/* get the capacity of the dv */
size_t DvCapacity(const dv_t* dv)
{
	assert(dv->base_p);

	return (dv->capacity);
}

/*free the memory of the vector*/
void DvDestroy(dv_t* dv)
{
	assert(dv->base_p);

	free(dv->base_p);
	dv->base_p = NULL;

	free(dv);
	dv = NULL;
}