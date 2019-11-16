/************************************
*									*
*									*
*	 	       TRIE       			*
*		             		        *
*									*
************************************/

#ifndef OL61_TRIE
#define OL61_TRIE

#include <limits.h> /* CHAR_BIT   */
#include <stddef.h> /* for size_t */

typedef struct trie_t trie_t;
typedef struct trie_node_t trie_node_t;
typedef int status_t;
typedef unsigned int number_t;
/* will not compile if number_t is less than 32 bits in total */
extern int array[sizeof(number_t) * CHAR_BIT - 31];

enum return_status
{
	SUCCESS,
	FAIL,
	MEMORY_ALLOCATION_FAILED,
	NODE_ALLREADY_EXISTS,
	NODE_DO_NOT_EXISTS,
	TRIE_IS_FULL
};

/*
* creates a tire
* Arguments: max valid depth.
* Return value: address to new trie, returns NULL on failure.
* NOTE: must use create trie before using insertion.
*/
trie_t *TrieCreateTrie(int max_depth);

/*
* destroy the whole trie
* Arguments: tries head.
* Return value: none, detory the whole trie.
*/
void TrieDestroy(trie_t *head);

/*
* insert new number(its binary numbers).
* Arguments: trie head, number to insert.
* Return value: status, 0 - success, 2 - alloc failed, 3 - already exists.  
*/
int TrieInsert(trie_t *head, number_t num);

/*
* remove a number from the trie.
* Arguments: trie head, and number to remove.
* Return value: 0 - success, 1 - failure, didnt find element to remove.
*/
int TrieRemove(trie_t *head, number_t num);

/*
* check if trie is empty.
* Arguments: tries head.
* Return value: status if the trie is empty or not, 1 - true(empty), 0 - false.
*/
int TrieIsEmpty(trie_t *head);

/*
* calculate tire size.
* Arguments: tries head.
* Return value: trie size.
*/
size_t TrieSize(trie_t *head);

/*
* return how much numbers we inserted to the trie.
* Arguments: tries head.
* Return value: how much numbers inseted to the trie.
*/
size_t TrieCountInsertedNums(trie_t *head);

/*
* calculate next free number in trie.
* Arguments: trie head.
* Return value: trie size in bytes.
* NOTE: on error (TRIE_IS_FULL, ALLOCATION_FAILURE), otherwise return SUCCESS.
*/
status_t TrieNextUnusedNumber(trie_t *head, const number_t num, 
	                          number_t *new_num);

/*
* calculate trie size in bytes.
* Arguments: trie head.
* Return value: trie size in bytes.
*/
size_t TrieCalcSize(trie_t *head);

#endif /* OL61_TRIE */