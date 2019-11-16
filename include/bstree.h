/********************************************/
/*											*/
/*					OL 61					*/
/*				binary sorted tree			*/
/*											*/
/********************************************/

#ifndef OL61_BSTREE
#define OL61_BSTREE

#include <stddef.h> /* size_t */
typedef struct bs_tree_t bs_tree_t;
typedef struct bst_info_t *bst_iter;
typedef int (*compare_func)(const void *data1, const void *data2, void *param);
/*the action function should not change the key */
typedef int (*action_func)(void *data, void *param);

/*
*	return value - pointer to the tree management struct
*	arguments - compare function, and parameters for it
*	this function creates a tree management struct
*/
bs_tree_t *BstCreate(compare_func cmp_func, void *param);

/*
*	return value - none
*	arguments - tree management struct
*	this function receives a tree management struct,
*	and frees its allocated memory
*/
void BstDestroy(bs_tree_t *tree);

/*
*	return value - iter to the new data
*	arguments - tree management struct, void *data to insert
*	this function receives a tree management struct
*	and insert a new data to it
*/
bst_iter BstInsert(bs_tree_t *tree, void *data);

/*
*	return value - iter to the wanted data
*	arguments - tree management struct, void *data to find
*	this function receives a tree management struct
*	and finds a given value and returns its iter
*/
bst_iter BstFind(bs_tree_t *tree, const void *data);

/*
*	return value - returns numbe of elements
*	arguments - tree management struct.
*	this function counts the number of elements in the tree.
*/
size_t BstCount(const bs_tree_t *tree);

/*
*	return value - element's data
*	arguments - tree management struct.
*	this function removes given element.
*/
void *BstRemove(bst_iter iter);

/*
*	return value - success / fail.
*	arguments - tree management struct, action function, param
*	this function performs the given action for each element.
*/
int BstForeach(bst_iter from, bst_iter to, action_func act_func, void *param);

/*
*	return value - empty/not empty.
*	arguments - tree management struct.
*	The function checks if the tree is empty or not.
*/
int BstIsEmpty(const bs_tree_t *tree);

/*
*	return value - iterator to the first data.
*	arguments - tree management struct.
*	The function returns the beginning of the tree.
*/
bst_iter BstBegin(bs_tree_t *tree);

/*
*	return value - iterator to the last data.
*	arguments - tree management struct.
*	The function returns the last data.
*/
bst_iter BstEnd(bs_tree_t *tree);

/*
*	return value - next iterator.
*	arguments - tree iteraor.
*	The function returns the next iterator.
*/
bst_iter BstNext(bst_iter iter);

/*
*	return value - previous iterator.
*	arguments - tree iteraor.
*	The function returns the previous iterator.
*/
bst_iter BstPrev(bst_iter iter);

/*
*	return value - iterator data.
*	arguments - tree iterator.
*	The function returns the data of given iterator.
*/
void *BstGetData(bst_iter iter);

int BstIsSameIter(bst_iter iter1, bst_iter iter2);

#endif /* OL61_BSTREE */