#include <assert.h>      /* assert    */
#include <string.h>      /* memset    */
#include <stdlib.h>      /* malloc    */

#include "hash_table.h"  /* my header */

struct h_table_t
{
	size_t num_of_buckets;
	hash_func_t hash_func;
	slist_node_t **table;
	compare_func_t cmp_func;
	void *param;
};

enum message
{
	SUCCESS = 0,
	NOT_EMPTY = 0,
	FALSE = 0,
	TRUE = 1,
	NODE_CREATION_FAILED = 1,
	EMPTY = 1,
	EMPTY_TABLE = 1,
	DUPLICATE_DATA = 2
};

/**************************** Non API Functions *****************************/

/******************************************/
static int FirstEncounter(slist_node_t **place_to_insert, void *const data)
{
	assert(data);

	*place_to_insert = SlistCreateNode(data, NULL);

	return (NODE_CREATION_FAILED * (NULL == *place_to_insert));
}

/******************************************/
static slist_node_t *SearchInList(slist_node_t *node, compare_func_t cmp_func,
		                          void *const param, void *const data)
{
	slist_node_t *runner = node;

	assert(cmp_func);
	assert(data);

	while (NULL != runner && cmp_func(runner->data, data, param))
	{
		runner = runner->next;
	}

	return (runner);
}

/******************************************/
static int AdditionalEncounters(slist_node_t *place_to_insert, 
	                            void *const data, h_table_t *h_table)
{
	int status = 0;

	assert(place_to_insert);
	assert(data);
	assert(h_table);

	if (NULL == SearchInList(place_to_insert, h_table->cmp_func,
		                     h_table->param, data))
	{
		slist_node_t *new_node = NULL;

		status = FirstEncounter(&new_node, data);
		if (status == NODE_CREATION_FAILED)
		{
			return (NODE_CREATION_FAILED);
		}

		SlistInsertAfter(place_to_insert, new_node);
	}
	else
	{
		status = DUPLICATE_DATA;
	}

	return (status);
}

/******************************************/
static void RemoveLastNode(slist_node_t *head)
{
	slist_node_t *runner = head;

	assert(head);
	assert(head->next);

	while (NULL != runner->next->next)
	{
		runner = runner->next;
	}

	SlistRemoveAfter(runner);
}
/**************************** API Functions **********************************/

/*
* receives table size, hash table func, compare func and params.
* creates a hash table.
* returns the hash table if succeeded, NULL otherwise.
* O(1).
*/
h_table_t *HashCreate(const size_t table_size, hash_func_t hash_func, 
					  compare_func_t cmp_func, void *const param)
{
	h_table_t *h_table = NULL;

	assert(hash_func);
	assert(cmp_func);
	assert(table_size > 1);

	h_table = malloc(sizeof(h_table_t));
	if (NULL == h_table)
	{
		return (NULL);
	}

	h_table->num_of_buckets = table_size;
	h_table->hash_func = hash_func;
	h_table->cmp_func = cmp_func;
	h_table->param = param;
	h_table->table = malloc(table_size * sizeof(slist_node_t));
	if (NULL == h_table->table)
	{
		free(h_table);
		h_table = NULL;

		return (NULL);
	}
	memset(h_table->table, 0, table_size * sizeof(slist_node_t));
	
	return (h_table);
}

/*
* receives a hash table and a data to insert.
* inserts the data to the hash table.
* returns status. 
* NOTE: no duplicated keys
* O(1) for average case. O(n) for worst case. 
*/
int HashInsert(h_table_t *h_table, void *const data)
{
	unsigned int hashed_index = 0;
	int status = 0;

	assert(h_table);
	assert(data);

	hashed_index = h_table->hash_func(data, h_table->param);
	if (NULL == (h_table->table[hashed_index]))
	{	
		status = FirstEncounter(&(h_table->table[hashed_index]), data);
		assert(0 == status);
	}
	else
	{
		status = AdditionalEncounters((h_table->table[hashed_index]),
		                              data, h_table);
		assert(0 == status);
	}

	return (status);
}

/*
* receives a hash table and destroys it.
* O(n)
*/
void HashDestroy(h_table_t *h_table)
{
	size_t i = 0;

	assert(h_table);

	for (i = 0; i < h_table->num_of_buckets; ++i)
	{
		if (h_table->table[i])
		{
			SlistFreeAll(h_table->table[i]);
			h_table->table[i] = NULL;
		}
	}

	free(h_table->table);
	h_table->table = NULL;
	free(h_table);
	h_table = NULL;
}

/*
* receives a hash table.
* returns a boolean value: 1 if the table is empty, 0 otherwise.
* O(1).
*/
int HashIsEmpty(const h_table_t *h_table)
{
	size_t i = 0;

	assert(h_table);

	for (i = 0; i < h_table->num_of_buckets; ++i)
	{
		if (h_table->table[i])
		{
			return (NOT_EMPTY);
		}
	}

	return (EMPTY);
}

/*
* receives a hash table and a key to find in the table.
* returns the data matches the key.
* O(n).
* Note: if the table empty or didnt find: return NULL, otherwise
* return its data
*/
void *HashFind(h_table_t *h_table, const void *const key)
{
	unsigned int hashed_index = 0;

	assert(h_table);
	assert(key);

	hashed_index = h_table->hash_func((void *const)key, h_table->param);

	if (TRUE == HashIsEmpty(h_table))
	{
		return NULL;
	}

	return ((SearchInList(h_table->table[hashed_index], h_table->cmp_func,
			              h_table->param, (void *const)key))->data);
}

/* 
* receives a hash table and an action function.
* Runs the function on all buckets in the hash table 
* O(n)
* NOTE: if table is empty - return 1, or if foreach fail for some reason
* itll return its status, or success
*/
int HashForEach(h_table_t *h_table, act_func_t action, void *param)
{
	size_t i = 0;
	int result = 0;

	assert(h_table);

	if (TRUE == HashIsEmpty(h_table))
	{
		return EMPTY_TABLE;
	}

	for (i = 0; (i < h_table->num_of_buckets) && (0 == result); ++i)
	{
		if (h_table->table[i])
		{
			result = SlistForEach(h_table->table[i], action, param);
		}
	}

	return (result);
}

/*
* receives a hash table.
* Counts the number of elements in the hash table (O(n))
*/
size_t HashSize(const h_table_t *h_table)
{
	size_t i = 0;
	size_t list_element_counter = 0;

	assert(h_table);

	for (i = 0; i < h_table->num_of_buckets; ++i)
	{
		if (h_table->table[i])
		{
			list_element_counter += SlistCount(h_table->table[i]);
		}
	}

	return (list_element_counter);
}

/*
* receives a hash table and a key.
* removes the data matches the key.
* returns the data removed.
* O(n).
*/
void *HashRemove(h_table_t *h_table, const void *const key)
{
	unsigned int hashed_index = 0;
	void *return_data = NULL;
	slist_node_t *node_to_remove = NULL;

	assert(h_table);
	assert(key);

	hashed_index = h_table->hash_func((void *const)key, h_table->param);
	node_to_remove = SearchInList(h_table->table[hashed_index],
	                              h_table->cmp_func,
			                      h_table->param, (void *const)key);
	if (NULL == node_to_remove)
	{
		return NULL;
	}
	else
	{
		return_data = node_to_remove->data;

		if (1 == SlistCount(h_table->table[hashed_index]))
		{
			free(node_to_remove);
			node_to_remove = NULL;
			h_table->table[hashed_index]= NULL;
		}
		else if (NULL == node_to_remove->next)
		{
			RemoveLastNode(h_table->table[hashed_index]);
			node_to_remove = NULL;
		}
		else
		{
			SlistRemove(node_to_remove);
		}
	}

	return return_data;
}