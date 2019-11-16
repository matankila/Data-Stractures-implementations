#include <stdlib.h> /* allocations */
#include <assert.h> /* asserts     */
#include "barr.h"   /* bits array  */
#include "trie.h"   /* my header   */

#define TRUE 1
#define FALSE 0
#define RIGHT 1
#define LEFT 0
#define GET_RIGHT_CHILD(head)((head)->child[RIGHT])
#define GET_LEFT_CHILD(head)((head)->child[LEFT])
#define IS_FULL(root)(FULL == (root)->status)
#define IS_LEAF(head) \
            (GET_LEFT_CHILD(head) == NULL && NULL == GET_RIGHT_CHILD(head))
#define IS_TWO_CHILDREN_LEAFS(head) \
            (TRUE == IS_LEAF(GET_LEFT_CHILD(head)) && \
             TRUE == IS_LEAF(GET_RIGHT_CHILD(head)))
#define IS_TWO_CHILDREN_FULL(head) \
            (GET_LEFT_CHILD(head)->status == FULL && \
	         GET_RIGHT_CHILD(head)->status == FULL)
#define HAS_ONE_CHILD(head) \
            ((GET_LEFT_CHILD(head) != NULL && \
             NULL == GET_RIGHT_CHILD(head)) || \
	        (GET_RIGHT_CHILD(head) != NULL && \
	         NULL == GET_LEFT_CHILD(head)))
#define HAS_RIGHT_CHILD(head) \
            (GET_RIGHT_CHILD(head) != NULL && NULL == GET_LEFT_CHILD(head))
#define HAS_LEFT_CHILD(head) \
            (GET_LEFT_CHILD(head) != NULL && NULL == GET_RIGHT_CHILD(head))

enum node_status
{
	LEAF,
	FULL,
	OTHER
};

struct trie_t
{
	trie_node_t *root;
	int max_depth;
};

struct trie_node_t
{
	trie_node_t *child[2];
	status_t status;
};

/******************************* Non API Function ****************************/

/********************************************/
static trie_node_t *TrieCreateNode()
{
	trie_node_t *node = malloc(sizeof(trie_node_t));

	if (!node)
	{
		return (NULL);
	}

	node->child[LEFT] = NULL;
	node->child[RIGHT] = NULL;
	node->status = LEAF;

	return (node);
}

/********************************************/
static void TrieDestroyChildren(trie_node_t *head)
{
	assert(head);
	assert(head->child[LEFT]);
	assert(head->child[RIGHT]);

	free (head->child[LEFT]);
	head->child[LEFT] = NULL;
	free (head->child[RIGHT]);
	head->child[RIGHT] = NULL;
}

/********************************************/
static int TrieCreateChildren(trie_node_t *head)
{
	assert(head);

	head->child[LEFT] = TrieCreateNode();
	head->child[RIGHT] = TrieCreateNode();
	if (NULL == head->child[LEFT] || NULL == head->child[RIGHT])
	{
		free(head->child[LEFT]);
		free(head->child[RIGHT]);
		head->child[LEFT] = NULL;
		head->child[RIGHT] = NULL;

		return (MEMORY_ALLOCATION_FAILED);
	}
	
	head->child[LEFT]->status = FULL;
	head->child[RIGHT]->status = FULL;

	return (SUCCESS);
}

/********************************************/
static status_t TrieUpdateStatus(trie_node_t *head, int index, int depth)
{
	status_t status = OTHER;

	if (TRUE == IS_LEAF(head))
	{
		status = LEAF;
	}
	else if (TRUE == HAS_ONE_CHILD(head))
	{
		status = OTHER;
	}
	else if ((IS_TWO_CHILDREN_FULL(head)) ||
	         ((IS_TWO_CHILDREN_LEAFS(head)) && (index == depth - 1)))
	{
		status = FULL;
	}

	head->status = status;

	return (status);
}

/********************************************/
static trie_node_t *TrieInsertRecursive(trie_node_t *node, number_t number, 
								        int index, status_t *status, int depth)
{
	int res = 0;

    if (NULL == node && index <= depth)
	{
		node = TrieCreateNode();
		if (NULL == node)
		{
			*status = MEMORY_ALLOCATION_FAILED;

			return (NULL);
		}
	}
	else if ((index > depth || FULL == node->status))
	{	
		return (NULL);
	}

	res = BitArrayIsOn(number, index);
	node->child[res] = TrieInsertRecursive(node->child[res], number, 
										   index + 1 , status, depth);
	if (FULL == TrieUpdateStatus(node, index, depth))
	{
		TrieDestroyChildren(node);
	}

	return (node);
}

/********************************************/
static trie_node_t *TrieRemoveRecursive(trie_node_t *head, number_t number, 
							            int index, status_t *status, int depth)
{
	int return_value = 0;
	int res = 0;

	if (NULL == head || index > depth)
	{
		return (NULL);
	}
	else if (index == depth)
	{
		free(head);
		head = NULL;

		return (NULL);
	}
	else if (FULL == head->status && index < depth)
	{
		return_value = TrieCreateChildren(head);
		if (SUCCESS != return_value)
		{
			*status = MEMORY_ALLOCATION_FAILED;

			return NULL;
		}
	}

	res = BitArrayIsOn(number, index);
	head->child[res] = TrieRemoveRecursive(head->child[res], number,
		                                   index + 1, status, depth);
	TrieUpdateStatus(head, index, depth);

	return (head);
}

/********************************************/
static int TrieIsNumExists(trie_node_t *head, number_t number, int depth)
{
	int res = 0;
	trie_node_t *runner = head;
	int index = 0;

	assert(head);

	for (index = 0; index <= depth; ++index)
	{
		if (NULL == runner)
		{
			return (FALSE);
		}
		if (FULL == runner->status || index == depth)
		{
			return (TRUE);
		}

		res = BitArrayIsOn(number, index);
		runner = runner->child[res];
	}

	return (FALSE);
}

/********************************************/
static size_t TrieCountRecursive(trie_node_t *trie, int max_depth)
{
	if (trie == NULL && 0 < max_depth)
	{
		return (0);
	}
	else if (FULL == trie->status)
	{
		return (1 << (max_depth - 1));
	}

	if (IS_LEAF(trie) && 1 == max_depth)
	{
		return 1;
	}
	else if (HAS_LEFT_CHILD(trie))
	{
		return (TrieCountRecursive(trie->child[LEFT], max_depth - 1));
	}
	else if (HAS_RIGHT_CHILD(trie))
	{
		return (TrieCountRecursive(trie->child[RIGHT], max_depth - 1));
	}

	return (TrieCountRecursive(trie->child[LEFT], max_depth - 1) +
		    TrieCountRecursive(trie->child[RIGHT], max_depth - 1));
}

/********************************************/
static void TrieDestoryRecursive(trie_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	TrieDestoryRecursive(node->child[LEFT]);
	TrieDestoryRecursive(node->child[RIGHT]);
	free(node);
	node = NULL;

}

/********************************************/
static size_t TrieSizeRecursive(trie_node_t *head)
{
	if (NULL == head)
	{
		return 0;
	}

	return (TrieSizeRecursive(head->child[LEFT]) + 
		    TrieSizeRecursive(head->child[RIGHT]) + 1);
}

/********************************************/
static number_t TrieGetDirection(trie_node_t **runner, unsigned int res, 
	                             int i, number_t new_num)
{
	*runner = (*runner)->child[res];
	if (!*runner)
	{
		*runner = TrieCreateNode();
	}

	new_num |= ((res) << i);

	return (new_num);
}

/********************************************/
static status_t TrieGetNext(trie_node_t *head, number_t num,
                            int depth, number_t *new_num)
{
	unsigned int res = 0;
	int i = 0;
	trie_node_t *runner = head;

	for (i = 0; i < depth && NULL != runner; ++i)
	{
		res = BitArrayIsOn(num, i);
		if (NULL == runner->child[res])
		{
			runner->child[res] = TrieCreateNode();
		}
		else if (FULL == runner->child[res]->status || i + 1 == depth)
		{
			*new_num = TrieGetDirection(&runner, !res, i, *new_num);
		}
		else
		{
			*new_num = TrieGetDirection(&runner, res, i, *new_num);
		}
	}

	return ((NULL == runner) ? (MEMORY_ALLOCATION_FAILED) : (SUCCESS));
}

/********************************* API Function ******************************/
/*
* creates a tire
* Arguments: max valid depth.
* Return value: address to new trie, returns NULL on failure.
* NOTE: must use create trie before using insertion.
*/
trie_t *TrieCreateTrie(int max_depth)
{
	trie_t *trie = malloc(sizeof(trie_t));

	if (!trie)
	{
		return NULL;
	}

	trie->root = TrieCreateNode();
	if (!trie->root)
	{
		return NULL;
	}

	trie->max_depth = max_depth;

	return trie;
}

/*
* destroy the whole trie
* Arguments: tries head.
* Return value: none, detory the whole trie.
*/
void TrieDestroy(trie_t *head)
{
	assert(head);
	assert(head->root);

	TrieDestoryRecursive(head->root);

	free(head);
	head = NULL;
}

/*
* insert new number(its binary numbers).
* Arguments: trie head, number to insert.
* Return value: status, 0 - success, 2 - alloc failed, 3 - already exists.  
*/
int TrieInsert(trie_t *head, number_t num)
{
	int i = 0;
	status_t status = SUCCESS;

	assert(head);
	assert(head->root);

	if (TRUE == TrieIsNumExists(head->root, num, head->max_depth))
	{
		return (NODE_ALLREADY_EXISTS);
	}

	TrieInsertRecursive(head->root, num, i, &status, head->max_depth);

	return (status);
}

/*
* remove a number from the trie.
* Arguments: trie head, and number to remove.
* Return value: 0 - success, 1 - failure, didnt find element to remove.
*/
int TrieRemove(trie_t *head, number_t num)
{
	int i = 0;
	status_t status = SUCCESS;

	assert(head);
	assert(head->root);

	if (FALSE == TrieIsNumExists(head->root, num, head->max_depth))
	{
		return (NODE_DO_NOT_EXISTS);
	}

	TrieRemoveRecursive(head->root, num, i, &status, head->max_depth);

	return (status);
}

/*
* check if trie is empty.
* Arguments: tries head.
* Return value: status if the trie is empty or not, 1 - true(empty), 0 - false.
*/
int TrieIsEmpty(trie_t *head)
{
	assert(head);
	assert(head->root);

	if (TRUE == IS_LEAF(head->root))
	{
		return TRUE;
	}

	return FALSE;
}

/*
* calculate tire size (nodes).
* Arguments: tries head.
* Return value: trie size.
*/
size_t TrieSize(trie_t *head)
{
	assert(head);
	assert(head->root);

	return (TrieSizeRecursive(head->root));
}

/*
* return how much numbers we inserted to the trie.
* Arguments: tries head.
* Return value: how much numbers inseted to the trie.
*/
size_t TrieCountInsertedNums(trie_t *head)
{
	assert(head);
	assert(head->root);

	return (TrieCountRecursive(head->root, head->max_depth + 1));
}

/*
* calculate next free number in trie.
* Arguments: trie head.
* Return value: trie size in bytes.
* NOTE: on error (TRIE_IS_FULL, ALLOCATION_FAILURE), otherwise return SUCCESS.
*/
status_t TrieNextUnusedNumber(trie_t *head, const number_t num, 
	                          number_t *new_num)
{
	status_t status = SUCCESS;

	assert(head);

	if (IS_FULL(head->root))
	{
		status = TRIE_IS_FULL;
	}
	else if (FALSE == TrieIsNumExists(head->root, num, head->max_depth))
	{
		*new_num = num;
	}
	else
	{
		status = TrieGetNext(head->root, num, head->max_depth, new_num);
	}
	
	return (status);
}

/*
* calculate trie size in bytes.
* Arguments: trie head.
* Return value: trie size in bytes.
*/
size_t TrieCalcSize(trie_t *head)
{
	assert(head);

	return (TrieSize(head) * sizeof(trie_node_t) + sizeof(trie_t));
}