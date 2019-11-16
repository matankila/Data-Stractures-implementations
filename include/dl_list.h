#ifndef OL61_DL_LIST_
#define OL61_DL_LIST_

#include <stddef.h>

/**********************************/
/* a pointer to is match function */
/* for DlistIsMatch               */
/* return: 1 - true, 0 - false    */
/**********************************/
typedef int (*is_match_func_t)(const void *data, const void *param);

/*a pointer to action function - for DlistForEach*/
typedef int (*act_func_t)(void *data, void *param);

typedef struct dlist_t dlist_t;

typedef struct dlist_node_t dlist_node_t;

typedef struct dlist_info_t* iter_t;

/***********************************/
/* create a dll list with          */
/* two dummies - head & tail. O(1) */
/***********************************/
dlist_t *DlistCreateList(void);

/* O(n) */
void DlistDestroy(dlist_t *dlist);

/************************************/
/* inserted iter in success, end of */
/* list in failure. O(1)            */
/************************************/
iter_t DlistInsert(dlist_t *dlist, iter_t iter, void *data);

/*******************************************/
/* erase the node in the iter address.     */
/* returns the next iter in the list. O(1) */
/*******************************************/
iter_t DlistErase(iter_t iter);

/***************************************/
/* do the compare function from        */
/* "from" until "to" (not neccesseraly */
/* all the list)include "from" but     */
/* exclude "to", if not found          */
/* return "to" . O(n)                  */
/***************************************/
iter_t DlistFind(iter_t from, iter_t to, is_match_func_t is_match, const void *param);

/*************************************************/
/* do the action function from "from"            */
/* until "to" (not neccesseraly all the list)    */
/* returns the exit status of the act_func. O(n) */
/*************************************************/
int DlistForEach(iter_t from, iter_t to, act_func_t act, void *param);

/* O(n) */
size_t DlistCount(const dlist_t *dlist);

/* boolean return value- 1 if empty, 0 if not. O(1) */
int DlistIsEmpty(const dlist_t *dlist);

/* returns the iter of the new inserted data. O(1) */
iter_t DlistPushFront(dlist_t *dlist, void *data);

/* returns the iter of the new inserted data. O(1) */
iter_t DlistPushBack(dlist_t *dlist, void *data);

/* returns the data of the poped node. O(1) */
void *DlistPopFront(dlist_t *dlist);

/* returns the data of the poped node. O(1) */
void *DlistPopBack(dlist_t *dlist);

/*********************************************/
/* returns the first valid node in the list. */
/* (after the dummy head) if the list is     */
/* empty lets the user know                  */
/* (end-of-list iter). O(1)                  */
/*********************************************/
iter_t DlistBegin(dlist_t *dlist);

/* O(1) */
iter_t DlistEnd(dlist_t *dlist);

/* O(1) */
iter_t DlistNext(iter_t iter);

/* O(1) */
iter_t DlistPrev(iter_t iter);

/* boolean return value - 1 is success, 0 if failure o(1) */
int IsSameIter(iter_t iter1, iter_t iter2);

/* O(1) */
void *DlistGetData(iter_t iter);

/********************************************/
/* from must be before to in the same list! */
/* splicing an empty list might cause       */
/* undefined behavior O(1)                  */
/********************************************/
void DlistSplice(iter_t dest, iter_t from, iter_t to);

#endif /* OL61_DL_LIST_ */