#include <assert.h> /* assert            */
#include <stdlib.h> /* memory allocation */
#include "bstree.h" /* my header         */

#define GET_RIGHT_CHILD(iter) ((bst_iter)(((tree_node_t *)iter)->children[1]))
#define GET_LEFT_CHILD(iter) ((bst_iter)(((tree_node_t *)iter)->children[0]))
#define GET_PARENT(iter) ((bst_iter)((tree_node_t *)iter)->parent)
#define GET_PARENT_RIGHT_CHILD(iter) (GET_RIGHT_CHILD(GET_PARENT(iter)))
#define GET_PARENT_LEFT_CHILD(iter) (GET_LEFT_CHILD(GET_PARENT(iter)))
#define IS_RIGHT_CHILD(iter) (iter == GET_PARENT_RIGHT_CHILD(iter))
#define IS_LEFT_CHILD(iter) (iter == GET_PARENT_LEFT_CHILD(iter))
#define IS_LEAF(iter) (!(GET_RIGHT_CHILD(iter)) && !(GET_LEFT_CHILD(iter)))
#define WHICH_CHILD_AM_I(iter) (IS_RIGHT_CHILD(iter))
#define HAS_RIGHT_CHILD(iter) (NULL != GET_RIGHT_CHILD(iter))
#define GET_LEFT_CHILD_TREE(tree) ((bst_iter)(tree->root.children[0]))

typedef struct tree_node_t tree_node_t;

struct tree_node_t
{
	tree_node_t *parent;
	tree_node_t *children[2];
	void *data;
};

struct bs_tree_t
{
	tree_node_t root;
	compare_func cmp_func;
	void *param;
};

static bst_iter BstMostLeft(bst_iter iter)
{
	bst_iter runner = NULL;

	assert(iter);

	runner = iter;
	while (GET_LEFT_CHILD(runner))
	{
		runner = GET_LEFT_CHILD(runner);
	}

	return (runner);
}

static bst_iter BstPairChildToParent(tree_node_t *where,
                                     void *data,
                                     int side,
                                     tree_node_t *who)
{
	where->children[side] = who;
	who->parent = where;
	who->data = data;
	who->children[0] = NULL;
	who->children[1] = NULL;

	return ((bst_iter)who);
}

/*
*	return value - pointer to the tree management struct
*	arguments - compare function, and parameters for it
*	this function creates a tree management struct
*/
bs_tree_t *BstCreate(compare_func cmp_func, void *param)
{
	bs_tree_t *tree = NULL;

	assert(cmp_func);

	tree = malloc(sizeof(bs_tree_t));
	if (tree)
	{
		tree->cmp_func         = cmp_func;
		tree->param            = param;
		tree->root.parent      = NULL;
		tree->root.children[0] = NULL;
		tree->root.children[1] = NULL;
		tree->root.data        = NULL;
	}

	return (tree);
}

/*
*	return value - none
*	arguments - tree management struct
*	this function receives a tree management struct,
*	and frees its allocated memory
*/
void BstDestroy(bs_tree_t *tree)
{
	bst_iter runner = NULL;
	bst_iter prev   = NULL;
	bst_iter begin  = BstBegin((bs_tree_t *)tree);
	bst_iter end    = BstEnd((bs_tree_t *)tree);

	assert(tree);

	runner = begin;
	while (!(BstIsSameIter(runner, end)))
	{
		prev = runner;
		runner = BstNext(runner);
		BstRemove(prev);
	}

	free(runner);
	runner = NULL;
}

/*
*	return value - iter to the new data
*	arguments - tree management struct, void *data to insert
*	this function receives a tree management struct
*	and insert a new data to it
*/
bst_iter BstInsert(bs_tree_t *tree, void *data)
{
	tree_node_t *new_node = NULL;
	bst_iter iter         = NULL;
	bst_iter prev         = NULL;
	int found             = 0;
	int ret               = 0;

	assert(tree);
	assert(data);

	new_node = malloc(sizeof(tree_node_t));
	if (new_node)
	{
		if (BstIsEmpty(tree))
		{
			BstPairChildToParent(&tree->root, data, 0, new_node);
		}
		else
		{
			iter = GET_LEFT_CHILD_TREE(tree);
			prev = iter;
			while (!found)
			{
				prev = iter;
				ret = tree->cmp_func(data, BstGetData(iter), tree->param);
				iter = (bst_iter)(((tree_node_t *)iter)->children[!ret]);
				if(!iter)
				{
					found = 1;
				}
			}
			BstPairChildToParent((tree_node_t *)prev, data, !ret, new_node);
		}
	}

	return ((bst_iter)new_node);
}

/*
*	return value - iter to the wanted data
*	arguments - tree management struct, void *data to find
*	this function receives a tree management struct
*	and finds a given value and returns its iter
*   NOTE: if not found return End(root)
*/
bst_iter BstFind(bs_tree_t *tree, const void *data)
{
	bst_iter runner = NULL;
	bst_iter end    = NULL;

	assert(tree);
	assert(data);

	runner = BstBegin((bs_tree_t *)tree);
	end    = BstEnd((bs_tree_t *)tree);

	while (!(BstIsSameIter(runner, end)))
	{
		if(tree->cmp_func(data, BstGetData(runner), tree->param) ==
		   tree->cmp_func(BstGetData(runner), data, tree->param))
		{
			break;
		}
		runner = BstNext(runner);
	}

	return (runner);
}

/*
*	return value - returns numbe of elements
*	arguments - tree management struct.
*	this function counts the number of elements in the tree.
*/
size_t BstCount(const bs_tree_t *tree)
{
	bst_iter runner = NULL;
	bst_iter begin  = NULL;
	bst_iter end    = NULL;
	size_t counter  = 0;

	assert(tree);

	begin = BstBegin((bs_tree_t *)tree);
	end   = BstEnd((bs_tree_t *)tree);

	for(runner = begin; !BstIsSameIter(runner, end); runner = BstNext(runner))
	{
		++counter;
	}

	return counter;
}

/*
*	return value - element's data
*	arguments - tree management struct.
*	this function removes given element.
*/
void *BstRemove(bst_iter iter)
{
	void *data               = NULL;
	tree_node_t *parent      = NULL;
	tree_node_t *curr_node   = (tree_node_t *)iter;
	int index                = 0;

	assert(iter);

	index  = WHICH_CHILD_AM_I(iter);
	parent = (tree_node_t *)GET_PARENT(iter);
	data   = BstGetData(iter);
	if (IS_LEAF(iter))
	{
		parent->children[index] = NULL;
	}
	else if (!GET_LEFT_CHILD(iter) || !GET_RIGHT_CHILD(iter))
	{
		parent->children[index] = curr_node->children[HAS_RIGHT_CHILD(iter)];
		curr_node->children[HAS_RIGHT_CHILD(iter)]->parent = parent; 
	}
	else
	{
		bst_iter most_left_iter = BstMostLeft(GET_RIGHT_CHILD(iter));
		tree_node_t *most_left = (tree_node_t *)most_left_iter;
		parent->children[index] = (tree_node_t *)GET_RIGHT_CHILD(iter);
		most_left->children[0] = (tree_node_t *)GET_LEFT_CHILD(iter);
		curr_node->children[0]->parent = most_left;
		curr_node->children[1]->parent  = curr_node->parent;
	}

	free(iter);
	iter = NULL;

	return (data);
}

/*
*	return value - success / fail.
*	arguments - tree management struct, action function, param
*	this function performs the given action for each element.
*/
int BstForeach(bst_iter from, bst_iter to, action_func act_func, void *param)
{
	bst_iter runner = NULL;
	size_t ret      = 0;

	assert(act_func);
	assert(to);
    assert(from);

	runner = from;
	while (!BstIsSameIter(runner, to) && !ret)
	{
		ret = act_func(BstGetData(runner), param);
		runner = BstNext(runner);
	}

	return (ret);
}

/*
*	return value - empty/not empty.
*	arguments - tree management struct.
*	The function checks if the tree is empty or not.
*/
int BstIsEmpty(const bs_tree_t *tree)
{	
	assert(tree);

	return (!GET_LEFT_CHILD_TREE(tree));
}

/*
*	return value - iterator to the first data.
*	arguments - tree management struct.
*	The function returns the beginning of the tree.
*/
bst_iter BstBegin(bs_tree_t *tree)
{
	bst_iter iter = NULL;

	assert(tree);

	iter = BstEnd(tree);
	while (GET_LEFT_CHILD(iter))
	{
		iter = GET_LEFT_CHILD(iter);
	}

	return (iter);
}

/*
*	return value - iterator to the last data.
*	arguments - tree management struct.
*	The function returns the last data.
*/
bst_iter BstEnd(bs_tree_t *tree)
{
	assert(tree);

	return ((bst_iter)&(tree->root));
}

/*
*	return value - next iterator.
*	arguments - tree iteraor.
*	The function returns the next iterator.
*/
bst_iter BstNext(bst_iter iter)
{
	assert(iter);

	if (GET_RIGHT_CHILD(iter))
	{
		iter = GET_RIGHT_CHILD(iter);
		while (iter && GET_LEFT_CHILD(iter))
		{
			iter = GET_LEFT_CHILD(iter);
		}

		return (iter);
	}
    else
    {
		while (IS_RIGHT_CHILD(iter))
		{
			iter = GET_PARENT(iter);
		}

		return ((GET_PARENT(iter)) ? (GET_PARENT(iter)) : (iter));	
	}
}

/*
*	return value - previous iterator.
*	arguments - tree iteraor.
*	The function returns the previous iterator.
*/
bst_iter BstPrev(bst_iter iter)
{
	assert(iter);

	if (GET_LEFT_CHILD(iter))
	{
		iter = GET_LEFT_CHILD(iter);
		while (GET_RIGHT_CHILD(iter))
		{
			iter = GET_RIGHT_CHILD(iter);
		}

		return (iter);
	}

	while (IS_LEFT_CHILD(iter))
	{
		iter = GET_PARENT(iter);
	}

	return ((GET_PARENT(iter)) ? (GET_PARENT(iter)) : (iter));	
}

/*
*	return value - iterator data.
*	arguments - tree iterator.
*	The function returns the data of given iterator.
*/
void *BstGetData(bst_iter iter)
{
	assert(iter);

	return (((tree_node_t *)iter)->data);
}

int BstIsSameIter(bst_iter iter1, bst_iter iter2)
{
	return (iter1 == iter2);
}