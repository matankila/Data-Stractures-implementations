#ifndef  OL61_AVL_TREE
#define  OL61_AVL_TREE

/************/
/*09.01.2019*/
/************/
#include <stddef.h> /*	size_t	*/

typedef struct avl_tree_t avl_tree_t;

typedef int (*cmp_func) (const void *internal_data, const void *external_data,
																void *param);
typedef int (*act_func_t)(void *internal_data, void *param);

/* Creates an avl tree (O(1))*/
/* NOTE: return NULL in case of failure */
avl_tree_t *AVLCreate(cmp_func comp, void *param);

/* Destroys the avl tree (O(n))*/
/* Post order implementation*/
void AVLDestroy(avl_tree_t *avl_tree);

/* Removes an element from the avl tree (O(log(n)))*/
void *AVLRemove(avl_tree_t *avl_tree, const void *data);

/* Inserts a new element to the avl tree (O(log(n)))*/
/* no duplicated keys*/
/* NOTE: SUCCESS = 0, ALLOCATION FAILURE = 1, DUPLICATE KEY = 2 */
int AVLInsert(avl_tree_t *avl_tree, void *data);

/* Returns the height of the avl tree (O(log(n)))*/
size_t AVLHeight(const avl_tree_t *avl_tree);

/* Counts the number of elements in the avl tree (O(n))*/
/*Pre-order implementation*/
size_t AVLSize(const avl_tree_t *avl_tree);

/* Checks if the avl tree is empty(O(1))*/
int AVLIsEmpty(const avl_tree_t *avl_tree);

/* Searchs an element in the avl tree (O(log(n)))*/
void *AVLFind(avl_tree_t *avl_tree, const void *data);

/* Runs a function on all elements in the avl tree (O(n))*/
/*In-order implementation*/
int AVLForEach(avl_tree_t *avl_tree,
			   int (*act_func_t)(void *internal_data, void *param),
			   void *param);

#endif /* OL61_AVL_TREE */