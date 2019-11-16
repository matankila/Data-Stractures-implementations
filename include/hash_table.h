/************/
/*14.01.2019*/
/************/

#ifndef OL61_HASH_TABLE
#define OL61_HASH_TABLE

#include <stddef.h> /*	size_t	*/
#include "linked_list.h" /* my header */

typedef struct h_table_t h_table_t;

typedef int (*compare_func_t) (const void *internal_data, 
	                           const void *external_data, void *param);

typedef unsigned int (*hash_func_t)(void *const data, void *const param);

/*
* receives table size, hash table func, compare func and params.
* creates a hash table.
* returns the hash table if succeeded, NULL otherwise.
* O(1).
*/
h_table_t *HashCreate(const size_t table_size, hash_func_t hash_func, 
					  compare_func_t cmp_func, void *const param);

/*
* receives a hash table and destroys it.
* O(n)
*/
void HashDestroy(h_table_t *h_table);

/*
* receives a hash table and a data to insert.
* inserts the data to the hash table.
* returns status. 
* NOTE: no duplicated keys
* O(1) for average case. O(n) for worst case. 
*/
int HashInsert(h_table_t *h_table, void *const data);

/*
* receives a hash table and a key.
* removes the data matches the key.
* returns the data removed.
* O(n).
*/
void *HashRemove(h_table_t *h_table, const void *const key);

/*
* receives a hash table.
* Counts the number of elements in the hash table (O(n))
*/
size_t HashSize(const h_table_t *h_table);

/*
* receives a hash table.
* returns a boolean value: 1 if the table is empty, 0 otherwise.
* O(1).
*/
int HashIsEmpty(const h_table_t *h_table);

/*
* receives a hash table and a key to find in the table.
* returns the data matches the key.
* O(n).
*/
void *HashFind(h_table_t *h_table, const void *const key);

/* 
* receives a hash table and an action function.
* Runs the function on all buckets in the hash table 
* O(n)
*/
int HashForEach(h_table_t *h_table, act_func_t action, void *param);

#endif /* OL61_HASH_TABLE */