#include <assert.h>  /* assert      */
#include <stdlib.h>  /* allocation  */
#include <string.h>  /* memcpy      */
#include "avltree.h" /* my header   */

#define SUCCESS 0
#define ALLOC_FAIL 1
#define DUPLICATE_KEY 2
#define TRUE 1
#define FALSE 0
#define ABS(number)((number < 0) ? (number * -1) : (number))

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

/***********************************************/
static int HasLeftChild(avl_tree_node_t *node)
{
	return (NULL != node && NULL != node->left);
}

/***********************************************/
static int HasRightChild(avl_tree_node_t *node)
{
	return (NULL != node && NULL != node->right);
}

/***********************************************/
static int IsLeaf(avl_tree_node_t *node)
{
	return ((NULL != node && FALSE == HasRightChild(node)) &&
	       (FALSE == HasLeftChild(node)));
}

/***********************************************/
static size_t CalcMaxHeight(avl_tree_node_t *current_root)
{
	if (NULL == current_root)
	{
		return 0;
	}
	else if (IsLeaf(current_root))
	{
		return 1;
	}
	else if (current_root->balance_factor > 0)
	{
		return (CalcMaxHeight((current_root)->left) + 1);
	}

	return (CalcMaxHeight((current_root)->right) + 1);
}

/***********************************************/
static int CalcBalance(avl_tree_node_t *node)
{	
	if (IsLeaf(node))
	{
		return 0;
	}
	else if (TRUE == HasLeftChild(node) && FALSE == HasRightChild(node))
	{
		return ((int)(CalcMaxHeight(node->left)));
	}
	else if (TRUE == HasRightChild(node) && FALSE == HasLeftChild(node))
	{
		return (0 - (int)CalcMaxHeight(node->right));
	}
	else
	{
		return (((int)CalcMaxHeight(node->left)) -
				((int)CalcMaxHeight(node->right)));
	}
}

static void UpdateBalanceAfterInsert(avl_tree_node_t *node, 
	                                 cmp_func cmp, 
	                                 void *param, void *data)
{
	int compare_result = 0;

	assert(cmp);
	assert(data);

	if (NULL == node)
	{
		return;
	}

	compare_result = cmp(data, node->data, param);
	if (compare_result == 0)
	{
		node->balance_factor = 0;

		return;
	}
	else if (0 < compare_result)
	{
		UpdateBalanceAfterInsert((node->right), cmp, param, data);
		node->balance_factor = CalcBalance(node);

		return;
	}

	UpdateBalanceAfterInsert((node->left), cmp, param, data);
	node->balance_factor = CalcBalance(node);

	return;	
}
/***********************************************/
static avl_tree_node_t **SearchNodeToInsert(avl_tree_node_t **node, 
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

	compare_result = cmp(data, (*node)->data, param);
	if (compare_result == 0)
	{
		return ((avl_tree_node_t **)0xDEADBEEF);
	}
	else if (0 < compare_result)
	{
		return (SearchNodeToInsert(&((*node)->right), cmp, param, data));
	}

	return (SearchNodeToInsert(&((*node)->left), cmp, param, data));
}

/***********************************************/
static avl_tree_node_t *InitNode(avl_tree_node_t *node, void *data)
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

/***********************************************/
static int CreateNewNode(avl_tree_node_t **node, void *data,
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

/***********************************************/
static size_t RecursiveSize(avl_tree_node_t *node)
{
	if(NULL == node)
	{
		return (0);
	}

	return ((RecursiveSize(node->left)) + (RecursiveSize(node->right)) + 1);
}

/***********************************************/
static void DestroyAll(avl_tree_node_t *node)
{
	assert(node);

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

/***********************************************/
static avl_tree_node_t *FindNodeElement(avl_tree_node_t *node,
                                        const void *data, 
                                        cmp_func cmp, void *param)
{
	int compare_result = 0;

	assert(data);
	assert(cmp);

	if (NULL == node)
	{
		return (NULL);
	}

	compare_result = cmp(data, node->data, param);
	if (0 == compare_result)
	{
		return (node);
	}
	else if (0 < compare_result)
	{
		return (FindNodeElement(node->right, data, cmp, param));
	}

	return (FindNodeElement(node->left, data, cmp, param));
}

/***********************************************/
static void *FindElement(avl_tree_node_t *node, const void *data, 
		                 cmp_func cmp, void *param)
{
	avl_tree_node_t *returned_node = NULL;

	returned_node = FindNodeElement(node, data, cmp, param);

	return ((NULL == returned_node) ? (NULL) : (returned_node->data));
}		

/***********************************************/
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

/***********************************************/
static void SwapNodes(avl_tree_node_t *node1, avl_tree_node_t *node2,
                      avl_tree_node_t *temp)
{
	assert(node1);
	assert(node2);

	memmove(temp, node1, sizeof(avl_tree_node_t));
	memmove(node1, node2, sizeof(avl_tree_node_t));
	memmove(node2, temp, sizeof(avl_tree_node_t));
}

/***********************************************/
static avl_tree_node_t *MostLeftChild(avl_tree_node_t *node)
{	
	assert(node);

	if (TRUE == IsLeaf(node))
	{
		return node;
	}

	return (MostLeftChild(node->left));
}

/***********************************************/
static void DeleteLeafRoot(avl_tree_node_t *node_to_delete)
{
	assert(node_to_delete);

	free(node_to_delete);
	node_to_delete = NULL;
}

/***********************************************/
static void DeleteLeaf(avl_tree_node_t *parent_of_delete_node, 
	                   avl_tree_node_t *node_to_delete)
{
	assert(parent_of_delete_node);
	assert(node_to_delete);

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

/***********************************************/
static void DeleteNodeWithRightSubTree(avl_tree_node_t *node_to_delete, 
	                                   avl_tree_node_t *right_node, 
	                                   avl_tree_node_t *node_for_swap, 
		                               avl_tree_node_t *most_left_child,
		                               avl_tree_node_t *left_node)
{
	assert(node_to_delete);
	assert(right_node);

	SwapNodes(node_to_delete, right_node, node_for_swap);
	free(right_node);
	right_node = NULL;
	most_left_child = MostLeftChild(node_to_delete);
	most_left_child->left = left_node;
}

/***********************************************/
static void DeleteNodeWithLeftSubTree(avl_tree_node_t *node_to_delete, 
	                                  avl_tree_node_t *left_node,
	                                  avl_tree_node_t *node_for_swap)
{
	assert(node_to_delete);
	assert(left_node);

	SwapNodes(node_to_delete, left_node, node_for_swap);
	free(left_node);
	left_node = NULL;
}

/***********************************************/
static void *DeleteNodeAndFixTree(avl_tree_node_t *node_to_delete,
	                              avl_tree_node_t *parent_of_delete_node)
{
	avl_tree_node_t *left_node = NULL;
	avl_tree_node_t *right_node = NULL;
	avl_tree_node_t *node_for_swap = NULL;
	avl_tree_node_t *most_left_child = NULL;
	void *data_to_return = NULL;

	assert(node_to_delete);
	assert(parent_of_delete_node);

	node_for_swap = malloc(sizeof(avl_tree_node_t));
	if (NULL == node_for_swap)
	{
		return NULL;
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
		                           node_for_swap, most_left_child, left_node);
	}
	else
	{
		DeleteNodeWithLeftSubTree(node_to_delete, left_node, node_for_swap);
	}
	free(node_for_swap);
	node_for_swap = NULL;

	return (data_to_return);
}

/***********************************************/
static avl_tree_node_t *FindParentOfNode(avl_tree_node_t *begin_node, 
	                                     avl_tree_node_t *node_child,
	                                     cmp_func cmp, void *param)
{
	assert(begin_node);
	assert(node_child);
	assert(cmp);

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

/***********************************************/
static avl_tree_node_t *avl_rotate_leftleft( avl_tree_node_t *node ) 
{
	avl_tree_node_t *original_head = node;
	avl_tree_node_t *new_head = NULL;

	assert(node);

	new_head = original_head->left;
	original_head->left = new_head->right;
	new_head->right = original_head;
	new_head->balance_factor = CalcBalance(new_head);
	original_head->balance_factor = CalcBalance(original_head);

	return(new_head);
}

/***********************************************/
static avl_tree_node_t *avl_rotate_leftright( avl_tree_node_t *node ) 
{
	avl_tree_node_t *original_head = node;
	avl_tree_node_t *new_head = NULL;
	avl_tree_node_t *c = NULL;

	assert(node);

	new_head = original_head->left;
	c = new_head->right;
	original_head->left = c->right;
	new_head->right = c->left; 
	c->left = new_head;
	c->right = original_head;
	new_head->balance_factor = CalcBalance(new_head);
	original_head->balance_factor = CalcBalance(original_head);

	return(c);
}

/***********************************************/
static avl_tree_node_t *avl_rotate_rightleft( avl_tree_node_t *node ) 
{
	avl_tree_node_t *original_head = node;
	avl_tree_node_t *new_head = NULL;
	avl_tree_node_t *c = NULL;

	assert(node);

	new_head = original_head->right;
	c = new_head->left;
	original_head->right = c->left;
	new_head->left = c->right; 
	c->right = new_head;
	c->left = original_head;
	new_head->balance_factor = CalcBalance(new_head);
	original_head->balance_factor = CalcBalance(original_head);

	return(c);
}

/***********************************************/
static avl_tree_node_t *avl_rotate_rightright( avl_tree_node_t *node ) 
{
	avl_tree_node_t *original_head = node;
	avl_tree_node_t *new_head = NULL;

	assert(node);

	new_head = original_head->right;
	original_head->right = new_head->left;
	new_head->left = original_head; 
	new_head->balance_factor = CalcBalance(new_head);
	original_head->balance_factor = CalcBalance(original_head);

	return(new_head);
}

/***********************************************/
static avl_tree_node_t *avl_balance_node( avl_tree_node_t *node ) 
{
	avl_tree_node_t *newroot = NULL;
	int balance_factor = CalcBalance(node);

	assert(node);

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
		if (CalcBalance(node->left) <= -1) 
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
		if (CalcBalance(node->right) >= 1)
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

/***********************************************/
static void avl_balance( avl_tree_t *tree ) {

	avl_tree_node_t *newroot = NULL;

	newroot = avl_balance_node(tree->root);
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
		return (DUPLICATE_KEY);
	}
	status = CreateNewNode((node_to_insert), data, 
		                   avl_tree->compare, avl_tree->param);
	UpdateBalanceAfterInsert(avl_tree->root,avl_tree->compare,
	                         avl_tree->param, data);
	avl_balance(avl_tree);

	return (status);
}

/* Counts the number of elements in the avl tree (O(n))*/
/*Pre-order implementation*/
size_t AVLSize(const avl_tree_t *avl_tree)
{
	size_t counter = 0;

	assert(avl_tree);

	counter = RecursiveSize(avl_tree->root);

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

	return (CalcMaxHeight(avl_tree->root));
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