#include <stdlib.h> /* allocations */ /**** asserts? ***/
#include "barr.h"   /* bits array  */
#include "trie.h"   /* my header   */

#define TRUE 1
#define FALSE 0
#define RIGHT 1
#define LEFT 0
#define GET_RIGHT_CHILD(head)((head)->child[RIGHT])
#define GET_LEFT_CHILD(head)((head)->child[LEFT])
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
	LEAF, /*** MAYBE FREE DESCRIBE THIS STATUS BETTER ***/
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
static void DestroyChildren(trie_node_t *head)
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
static int CreateChildren(trie_node_t *head)
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
static status_t UpdateStatus(trie_node_t *head, int index, int depth)
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
	else if (IS_TWO_CHILDREN_FULL(head))
	{
		status = FULL;
	}
	else if (IS_TWO_CHILDREN_LEAFS(head) && index == depth - 1) 
	{ /*** the check here is a unecessary. the check for IS_TWO_CHILDREN_FULL(head)
		   should be enough ***/
		status = FULL;
	}

	head->status = status;

	return (status);
}

/********************************************/
static trie_node_t *TrieInsertRecursive(trie_node_t *head, number_t number, 
								        int index, status_t *status, int depth)
{		/*** why do you call a node "head"? it's a term from singly and double linked list ***/
	int res = 0;

    if (NULL == head && index <= depth)
	{
		head = TrieCreateNode();
		if (NULL == head)
		{
			*status = MEMORY_ALLOCATION_FAILED;

			return (NULL);
		}
	}
	else if ((index > depth || head->status == FULL))
	{	/*** you don't want to update the status here? ***/
		return (NULL);
	}
	/*** res is not a good name for this variable. it represent a bit status in a specific index ***/
	res = BitArrayIsOn(number, index);
	head->child[res] = TrieInsertRecursive(head->child[res], number, 
										   index + 1 , status, depth);
	if (UpdateStatus(head, index, depth) == FULL)
	{
		DestroyChildren(head);
	}

	return (head);
}

/********************************************/
static trie_node_t *TrieRemoveRecursive(trie_node_t *head, number_t number, 
							            int index, status_t *status, int depth)
{
	int return_value = 0;
	int res = 0;

	if (NULL == head || index > depth)
	{ /*** you dont want to update the status here? ***/
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
		return_value = CreateChildren(head);
		assert(SUCCESS == return_value); /*** this is not a case for assertion
		                                       you need to do a proper return status check ***/
	}                                               

	res = BitArrayIsOn(number, index);
	head->child[res] = TrieRemoveRecursive(head->child[res], number,
		                                   index + 1, status, depth);
	UpdateStatus(head, index, depth);
	/*** if a node becomes free you don't remove it?  you change***/
	return (head);
}

/********************************************/
static int IsNumExistsInTrie(trie_node_t *head, number_t number, int depth)
{
	int res = 0;
	trie_node_t *runner = head;
	int index = 0;

	assert(head);

	for (index = 0; index <= depth; ++index)
	{
		if (runner == NULL)
		{
			return (FALSE);
		}
		if (runner->status == FULL || index == depth)
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
static number_t GetDir(trie_node_t **runner, unsigned int res, 
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
static number_t GetNext(trie_node_t *head, number_t num, int depth)
{
	unsigned int res = 0;
	int i = 0;
	number_t new_num = 0;
	trie_node_t *runner = head;

	if (runner->status == FULL)
	{
		return new_num;
	}

	for (i = 0; i < depth && NULL != runner; ++i)
	{
		res = BitArrayIsOn(num, i);
		if (NULL == runner->child[res])
		{
			runner->child[res] = TrieCreateNode();
		}
		else if (FULL == runner->child[res]->status)
		{
			new_num = GetDir(&runner, !res, i, new_num);
		}
		else
		{
			new_num = GetDir(&runner, res, i, new_num);
		}
	}

	return ((NULL == runner) ? (0) : (new_num));
}

/********************************* API Function ******************************/
/*
* creates the root
* Arguments: max valid depth.
* Return value: address to new root, returns NULL on failure.
* NOTE: must use create root before creating a node.
*/
trie_t *TrieCreateRoot(int max_depth)
{ /*** I'ts confusing to call the management struct "root" when it's not actually the root ***/
	trie_t *root = malloc(sizeof(trie_t));
/*** remove space ***/
	if (!root)
	{
		return NULL;
	}

	root->root = TrieCreateNode();
	if (!root->root)
	{
		return NULL;
	}

	root->max_depth = max_depth;

	return root;
}

/*
* creates new nodes in the trie.
* Arguments: none.
* Return value: address of new created node, return NULL on failure.
*/
trie_node_t *TrieCreateNode()
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
/*** remove space ***/
	if (!node)
	{
		return (NULL);
	}

	node->child[LEFT] = NULL;
	node->child[RIGHT] = NULL;
	node->status = LEAF;

	return (node);
}

/*
* destroy the whole trie
* Arguments: tries head.
* Return value: none, detory the whole trie.
*/
void TrieDestory(trie_t *head) /*** you mean TrieDestroy ;) ***/
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
	/*** why do you check for IS_LEAF here? if the root status is OTHER? what happen? 
	      In general, use macros and static functions that have explicit names of what you are doing ***/
	if (FALSE == IS_LEAF(head->root) && 
		TRUE == IsNumExistsInTrie(head->root, num, head->max_depth))
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

	if (TRUE == IS_LEAF(head->root) || 
		FALSE == IsNumExistsInTrie(head->root, num, head->max_depth))
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
* NOTE: on error (full trie, creation error) will return 0 which is not valid.
*/
number_t TrieNextUnusedNumber(trie_t *head, const number_t num)
{
	number_t new_num = 0;

	assert(head);

	if (FALSE == IsNumExistsInTrie(head->root, num, head->max_depth))
	{
		return num;
	}

	new_num = GetNext(head->root, num, head->max_depth);

	return (new_num);
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