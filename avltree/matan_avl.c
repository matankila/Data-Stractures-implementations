!/*** in general, you should put a comment between the function just for separtion, and explenation*/
#include <stdio.h>   /* IO fucntion */ !/*** why do you need this? there are no prints...*/
#include <assert.h>  /* assert      */
#include <stdlib.h>  /* allocation  */
#include <string.h>  /* memcpy      */
#include "avltree.h" /* my header   */

#define SUCCESS 0
#define ALLOC_FAIL 1
#define DUP_KEY 2 !/*** with the 1st read, I don't know what this DUP_KEY is...*/
#define TRUE 1
#define FALSE 0

typedef struct avl_tree_node_t avl_tree_node_t;

struct avl_tree_node_t
{
	void *data;
	avl_tree_node_t *left;
	avl_tree_node_t *right;
	int balance_factor;
};

struct avl_tree_t
{
	avl_tree_node_t *root;
	cmp_func compare;
	void *param; 
};

/****************************** Non API fucntions ****************************/

static int HasLeftChild(avl_tree_node_t *node)
{
	assert(node);

	return (NULL != node->left);
}

static int HasRightChild(avl_tree_node_t *node)
{
	assert(node);

	return (NULL != node->right);
}

static int IsLeaf(avl_tree_node_t *node)
{
	assert(node);

	return ((FALSE == HasRightChild(node)) && (FALSE == HasLeftChild(node)));
}

static size_t maxDepth(avl_tree_node_t *node)  !/*the name by convention must start with capital letter*/
{ 
    if (NULL == node) 
    {
        return (0);
    } 
    else 
    {
        size_t leftDepth  = maxDepth(node->left);
        size_t rightDepth = maxDepth(node->right);

        if (leftDepth > rightDepth )
        {
            return (leftDepth + 1);
        }
        else
        {
            return (rightDepth + 1);
        }
    }
}

static int GetBalance(avl_tree_node_t *node)
{
	if (node == NULL) 
	{
        return (0); 
	}

    return (maxDepth(node->left) - maxDepth(node->right)); 
}

static avl_tree_node_t **SearchNodeToInsert(avl_tree_node_t **node,  !/* you have a habbit of complicating simple things*/
	                                        cmp_func cmp, 
	                                        void *param, void *data)
{
	int compare_result = 0;

	assert(cmp);
	assert(data);

	if (NULL == *node)
	{
		return (node);
	}

	compare_result = cmp(data, (*node)->data, param); !/*** usually we check the node that is on the tree than data*/

	if (compare_result == 0)
	{
		return ((avl_tree_node_t **)0xDEADBEEF); !/*** this is only a showoff :-), but you could easly return the node*/
	}
	else if (0 < compare_result)
	{
		(*node)->balance_factor = GetBalance(*node);

		return (SearchNodeToInsert(&((*node)->right), cmp, param, data));
	}

	(*node)->balance_factor = GetBalance(*node);

	return (SearchNodeToInsert(&((*node)->left), cmp, param, data));
}

static avl_tree_node_t *InitNode(avl_tree_node_t *node, void *data) !/*** this is Create and Init*/
{
	assert(data);

	node = malloc(sizeof(avl_tree_node_t));

	if(NULL == node)
	{
		return (NULL);
	}

	node->right = NULL;
	node->left = NULL;
	node->data = data;
	node->balance_factor = 0;

	return (node);
}

static int CreateNewNode(avl_tree_node_t **node, void *data, !/*this is basicly Create and Init*/
                         cmp_func cmp, void *param)
{
	assert(data);
	assert(cmp);

	(void)param;

	*node = InitNode(*node, data);

	if (NULL == *node)
	{
		return (ALLOC_FAIL);
	}

	return (SUCCESS);
}

static size_t ElementCounter(avl_tree_node_t *node) !/**The name of the function is not so helpfull, you can find a better name*/
{
	if(NULL == node)
	{
		return (0);
	}
	return ((ElementCounter(node->left)) + (ElementCounter(node->right)) + 1);
}

static void DestroyAll(avl_tree_node_t *node)
{
	if (TRUE == HasLeftChild(node))
	{
		DestroyAll(node->left);
	}

	if (TRUE == HasRightChild(node))
	{
		DestroyAll(node->right);
	}

	free(node);
	node = NULL;

	return ;
}

static avl_tree_node_t *FindNodeElement(avl_tree_node_t *node,
                                        const void *data, 
                                        cmp_func cmp, void *param)
{
	int compare_result = 0;
	!/***how about asserts*/
	if (NULL == node)
	{
		return (NULL);
	}

	compare_result = cmp(data, node->data, param);

	if (0 == compare_result)
	{
		return ((void *)node); !/*** why the case is neccecary?*/
	}
	else if (0 < compare_result)
	{
		return (FindNodeElement(node->right, data, cmp, param));
	}

	return (FindNodeElement(node->left, data, cmp, param));
}

static void *FindElement(avl_tree_node_t *node, const void *data, 
		                 cmp_func cmp, void *param)
{
	avl_tree_node_t *returned_node = NULL;
	!/***how about asserts*/
	returned_node = FindNodeElement(node, data, cmp, param);

	return ((NULL == returned_node) ? (NULL) : (returned_node->data));
}		

static int DoActionForAllElements(avl_tree_node_t *node, 
	                              act_func_t act, void *param)
{
	if (NULL == node)
	{
		return (0);
	}

	return (DoActionForAllElements(node->left, act, param) || 
		    act(node->data, param) ||
		    DoActionForAllElements(node->right, act, param));
}

static void SwapNodes(avl_tree_node_t *data1, avl_tree_node_t *data2, !/**do you swap data? or do you swap nodes*/
                          avl_tree_node_t *temp)
{
	memmove(temp, data1, sizeof(avl_tree_node_t));
	memmove(data1, data2, sizeof(avl_tree_node_t));
	memmove(data2, temp, sizeof(avl_tree_node_t));
}

static avl_tree_node_t *MostLeftChild(avl_tree_node_t *node)
{	
	if (TRUE == IsLeaf(node))
	{
		return node;
	}

	return (MostLeftChild(node->left));
}!/***spaces*/
static void DeleteLeafRoot(avl_tree_node_t *node_to_delete)
{
	free(node_to_delete);
	node_to_delete = NULL;
}!/***spaces*/
static void DeleteLeaf(avl_tree_node_t *parent_of_delete_node, 
	                   avl_tree_node_t *node_to_delete)
{
	if (parent_of_delete_node->right == node_to_delete)
	{
		parent_of_delete_node->right = NULL;
	}
	else
	{
		parent_of_delete_node->left = NULL;
	}

	free(node_to_delete);
	node_to_delete = NULL;
}

static void DeleteNodeWithRightSubTree(avl_tree_node_t *node_to_delete, 
	                                   avl_tree_node_t *right_node, 
	                                   avl_tree_node_t *data_for_swap, 
		                               avl_tree_node_t *most_left_child,
		                               avl_tree_node_t *left_node)
{
	SwapNodes(node_to_delete, right_node, data_for_swap);
	free(right_node);
	right_node = NULL;
	most_left_child = MostLeftChild(node_to_delete);
	most_left_child->left = left_node;
}

static void DeleteNodeWithLeftSubTree(avl_tree_node_t *node_to_delete, 
	                                  avl_tree_node_t *left_node,
	                                  avl_tree_node_t * data_for_swap) !/***Space*/
{
	SwapNodes(node_to_delete, left_node, data_for_swap);
	free(left_node);
	left_node = NULL;
}


static void *DeleteNodeAndFixTree(avl_tree_node_t *node_to_delete,
	                              avl_tree_node_t *parent_of_delete_node)
{
	avl_tree_node_t *left_node = NULL;
	avl_tree_node_t *right_node = NULL;
	avl_tree_node_t *data_for_swap = NULL;
	avl_tree_node_t *most_left_child = NULL;
	void *data_to_return = NULL;
	!/***asserts!*/
	data_for_swap = malloc(sizeof(avl_tree_node_t));

	if(NULL == data_for_swap)
	{
		return (NULL);
	}

	data_to_return = node_to_delete->data;
	left_node = node_to_delete->left;
	right_node = node_to_delete->right;

	if (TRUE == IsLeaf(node_to_delete) && NULL == parent_of_delete_node)
	{
		DeleteLeafRoot(node_to_delete);
	}
	else if (TRUE == IsLeaf(node_to_delete) && NULL != parent_of_delete_node)
	{
		DeleteLeaf(parent_of_delete_node, node_to_delete);
	}
	else if (TRUE == HasRightChild(node_to_delete))
	{
		DeleteNodeWithRightSubTree(node_to_delete, right_node,
		                           data_for_swap, most_left_child, left_node);
	}
	else
	{
		DeleteNodeWithLeftSubTree(node_to_delete, left_node, data_for_swap);
	}

	free(data_for_swap);
	data_for_swap = NULL;

	return (data_to_return);
}

static avl_tree_node_t *FindParentOfNode(avl_tree_node_t *begin_node, !/***begin_node is not a good name*/
	                                     avl_tree_node_t *node_child,
	                                     cmp_func cmp, void *param)
{
	!/***asserts?*/
	if (NULL == begin_node)
	{
		return (NULL);
	}
	else if (begin_node == node_child)
	{
		return (NULL);
	}
	else if (begin_node->left == node_child || begin_node->right == node_child)
	{
		return (begin_node);
	}
	else if (0 < cmp(node_child->data, begin_node->data, param))
	{
		return (FindParentOfNode(begin_node->right, node_child, cmp, param));
	}

	return (FindParentOfNode(begin_node->left, node_child, cmp, param)); 
}

static avl_tree_node_t *avl_rotate_leftleft( avl_tree_node_t *node ) 
{
	!/***WOW BAD NAMES!!!*/
 	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->left;
	a->left = b->right;
	b->right = a;

	return( b );
}

static avl_tree_node_t *avl_rotate_leftright( avl_tree_node_t *node ) 
{
		!/***WOW BAD NAMES!!!*/
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->left;
	avl_tree_node_t *c = b->right;
	a->left = c->right;
	b->right = c->left; 
	c->left = b;
	c->right = a;

	return( c );
}

static avl_tree_node_t *avl_rotate_rightleft( avl_tree_node_t *node ) 
{	!/***WOW BAD NAMES!!!*/
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->right;
	avl_tree_node_t *c = b->left;
	a->right = c->left;
	b->left = c->right; 
	c->right = b;
	c->left = a;

	return( c );
}

static avl_tree_node_t *avl_rotate_rightright( avl_tree_node_t *node ) 
{
		!/***WOW BAD NAMES!!!*/
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->right;
	a->right = b->left;
	b->left = a; 

	return( b );
}

static avl_tree_node_t *avl_balance_node( avl_tree_node_t *node ) 
{
	avl_tree_node_t *newroot = NULL;
	int balance_factor = GetBalance(node);

	if (node->left)
	{
		node->left = avl_balance_node(node->left);
	}

	if (node->right) 
	{
		node->right = avl_balance_node(node->right);
	}

	if (balance_factor >= 2) 
	{
		if (GetBalance(node->left) <= -1) 
		{
			newroot = avl_rotate_leftright(node);
		}
		else 
		{
			newroot = avl_rotate_leftleft(node);
		}

	} 
	else if (balance_factor <= -2) 
	{
		if (GetBalance(node->right) >= 1)
		{
			newroot = avl_rotate_rightleft(node);
		}
		else 
		{
			newroot = avl_rotate_rightright(node);
		}
	} 
	else 
	{
		newroot = node;
	}

	return(newroot);	
}

static void avl_balance( avl_tree_t *tree ) {

	avl_tree_node_t *newroot = NULL;

	newroot = avl_balance_node( tree->root );

	if( newroot != tree->root )  
	{
		tree->root = newroot; 
	}
}

/******************************** API fucntions ******************************/
/* Creates an avl tree (O(1))*/
/* NOTE: return NULL in case of failure */
avl_tree_t *AVLCreate(cmp_func comp, void *param)
{
	avl_tree_t *avl = NULL;

	assert(comp);

	avl = malloc(sizeof(avl_tree_t));

	if (NULL == avl)
	{
		return (NULL);
	}

	avl->root = NULL;
	avl->compare = comp;
	avl->param = param;

	return (avl);
}

/* Inserts a new element to the avl tree (O(log(n)))*/
/* no duplicated keys*/
/* NOTE: SUCCESS = 0, ALLOCATION FAILURE = 1, DUPLICATE KEY = 2 */
int AVLInsert(avl_tree_t *avl_tree, void *data)
{
	int status = SUCCESS;
	avl_tree_node_t **node_to_insert = NULL;

	assert(avl_tree);
	assert(data);

	if (AVLIsEmpty(avl_tree))
	{
		status = CreateNewNode(&(avl_tree->root), data, 
		                       avl_tree->compare, avl_tree->param);

		return (status);
	}

	node_to_insert = SearchNodeToInsert(&(avl_tree->root), avl_tree->compare,
	                                    avl_tree->param, data);

	if((avl_tree_node_t **)0xDEADBEEF == node_to_insert)
	{
		return (DUP_KEY); 
	}

	status = CreateNewNode((node_to_insert), data, 
		                   avl_tree->compare, avl_tree->param);
	avl_balance(avl_tree);

	return (status);
}

/* Counts the number of elements in the avl tree (O(n))*/
/*Pre-order implementation*/
size_t AVLSize(const avl_tree_t *avl_tree)
{
	size_t counter = 0;

	assert(avl_tree);

	counter = ElementCounter(avl_tree->root);

	return (counter);
}

/* Checks if the avl tree is empty(O(1))*/
int AVLIsEmpty(const avl_tree_t *avl_tree)
{
	assert(avl_tree);

	return (NULL == avl_tree->root);
}

/* Destroys the avl tree (O(n))*/
/* Post order implementation*/
void AVLDestroy(avl_tree_t *avl_tree)
{
	assert(avl_tree);

	DestroyAll(avl_tree->root);
	free(avl_tree);
	avl_tree = NULL;
}

/* Returns the height of the avl tree (O(log(n)))*/
size_t AVLHeight(const avl_tree_t *avl_tree)
{
	assert(avl_tree);

	return (maxDepth(avl_tree->root));
}

/* Searchs an element in the avl tree (O(log(n)))*/
void *AVLFind(avl_tree_t *avl_tree, const void *data)
{
	assert(avl_tree);
	assert(data);

	return (FindElement(avl_tree->root, data, 
		               avl_tree->compare, avl_tree->param));
}

/* Runs a function on all elements in the avl tree (O(n))*/
/*In-order implementation*/
int AVLForEach(avl_tree_t *avl_tree, act_func_t act, void *param)
{
	assert(avl_tree);
	assert(act);

	return (DoActionForAllElements(avl_tree->root, act, param));
}

/* Removes an element from the avl tree (O(log(n)))*/
void *AVLRemove(avl_tree_t *avl_tree, const void *data)
{
	avl_tree_node_t *node_to_delete = NULL;
	avl_tree_node_t *parent_of_delete_node = NULL;
	void *remove_return_value = NULL;

	assert(avl_tree);
	assert(data);

	node_to_delete = FindNodeElement(avl_tree->root, data,
	                                 avl_tree->compare, avl_tree->param);
	parent_of_delete_node = FindParentOfNode(avl_tree->root, node_to_delete,
		                                     avl_tree->compare, 
		                                     avl_tree->param);

	if (NULL == node_to_delete)
	{
		return (NULL);
	}

	remove_return_value = (DeleteNodeAndFixTree(node_to_delete,
		                                        parent_of_delete_node));
	avl_balance(avl_tree);

	return remove_return_value;
}