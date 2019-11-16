#include <assert.h>  /* assert */
#include <stdlib.h>  /* allocation */
#include "dl_list.h" /* my header */

typedef struct dlist_node_t node_t;

struct dlist_node_t
{
	void *data;
	node_t *next;
	node_t *prev;
};

struct dlist_t
{
	node_t head;
	node_t tail;
};

/*create a dll list with two dummies - head & tail. O(1)*/
dlist_t *DlistCreateList(void)
{
	dlist_t *ptr = NULL;

	ptr = malloc(sizeof(dlist_t));
	if (!ptr)
	{
		return NULL;
	}

	(ptr->head).data = NULL;
	(ptr->head).prev = NULL;
	(ptr->head).next = &(ptr->tail);
	(ptr->tail).data = NULL;
	(ptr->tail).next = NULL;
	(ptr->tail).prev = &(ptr->head);

	return ptr;
}

/*O(n)*/
void DlistDestroy(dlist_t *dlist)
{
	iter_t i     = NULL;
	iter_t size  = NULL;

	assert(dlist);

	size  = DlistEnd(dlist);
	i     = DlistBegin(dlist);

	while (!IsSameIter(i, size))
	{
		i = DlistNext(i);
		free(DlistPrev(i));
	}

	free(dlist);
	dlist = NULL;
}

/*inserted iter in success, end of list in failure. O(1)*/
iter_t DlistInsert(dlist_t *dlist, iter_t iter, void *data)
{
	node_t *insert_val = NULL;

	assert(dlist);

	insert_val = malloc(sizeof(node_t));
	if (!insert_val)
	{
		return (DlistEnd(dlist));
	}

	insert_val->data = data;
	insert_val->next = (node_t*)iter;
	insert_val->prev = ((node_t*)iter)->prev;

	((node_t *)iter)->prev->next = insert_val;
	((node_t *)iter)->prev = insert_val;

	return (iter_t)insert_val;
}

/*erase the node in the iter address. returns the next iter in the list. O(1)*/
iter_t DlistErase(iter_t iter)
{
	iter_t next_node = NULL;
	iter_t prev_node = NULL;

	assert(iter);

	next_node = DlistNext(iter);
	prev_node = DlistPrev(iter);

	((node_t*)next_node)->prev = (node_t*)prev_node;
	((node_t*)prev_node)->next = (node_t*)next_node;

	free(iter);
	iter = NULL;

	return next_node;
}

/*O(n)*/
size_t DlistCount(const dlist_t *dlist)
{
	iter_t i     = NULL;
	iter_t size  = NULL;
	size_t count = 0;

	assert(dlist);

	size  = DlistEnd((dlist_t *)dlist);
	i     = DlistBegin((dlist_t *)dlist);

	while (!IsSameIter(i, size))
	{
		++count;
		i = DlistNext(i);
	}

	return count;
}

/*boolean return value- 1 if empty, 0 if not. O(1)*/
int DlistIsEmpty(const dlist_t *dlist)
{
	assert(dlist);

	return ((dlist->head).next == &(dlist->tail) && (dlist->tail).prev == &(dlist->head)); 
}

/*returns the iter of the new inserted data. O(1)*/
iter_t DlistPushFront(dlist_t *dlist, void *data)
{
	iter_t new_iter = NULL;

	assert(dlist);

	new_iter = DlistInsert(dlist, DlistBegin(dlist), data);

	return new_iter;
}

/*returns the iter of the new inserted data. O(1)*/
iter_t DlistPushBack(dlist_t *dlist, void *data)
{
	iter_t new_iter = NULL;

	assert(dlist);

	new_iter = DlistInsert(dlist, DlistEnd(dlist), data);

	return new_iter;	
}

/*returns the data of the poped node. O(1)*/
void *DlistPopFront(dlist_t *dlist)
{
	iter_t first_elem = NULL;
	void *first_val   = NULL;

	assert(dlist);

	first_elem = DlistBegin(dlist);
	first_val  = DlistGetData(first_elem);

	DlistErase(first_elem);

	return first_val;
}

/*returns the data of the poped node. O(1)*/
void *DlistPopBack(dlist_t *dlist)
{
	iter_t last_elem = NULL;
	void *last_val   = NULL;

	assert(dlist);

	last_elem = DlistEnd(dlist);
	last_elem = DlistPrev(last_elem);
	last_val  = DlistGetData(last_elem);

	DlistErase(last_elem);

	return last_val;
}

/*returns the first valid node in the list. (after the dummy head)
if the list is empty lets the user know (end-of-list iter). O(1)*/
iter_t DlistBegin(dlist_t *dlist)
{
	iter_t iter = NULL;

	assert(dlist);

	iter = (iter_t)((dlist->head).next);

	return iter;
}

/*o(1)*/
iter_t DlistEnd(dlist_t *dlist)
{
	iter_t iter = NULL;

	assert(dlist);

	iter = (iter_t)&((dlist)->tail);

	return iter;
}

/*o(1)*/
iter_t DlistNext(iter_t iter)
{
	return ((iter_t)(((node_t *)iter)->next));
}

/*o(1)*/
iter_t DlistPrev(iter_t iter)
{	
	return ((iter_t)(((node_t *)iter)->prev));
}

/*boolean return value - 1 is true, 0 if false o(1)*/
int IsSameIter(iter_t iter1, iter_t iter2)
{
	return ((node_t *)iter1 == (node_t *)iter2);
}

/*o(1)*/
void *DlistGetData(iter_t iter)
{

	return (((node_t *)(iter))->data);
}

/*from must be before to in the same list!
splicing an empty list might cause undefined behavior O(1) */
void DlistSplice(iter_t dest, iter_t from, iter_t to)
{
	iter_t before_from = NULL;
	iter_t before_to   = NULL;
	iter_t dest_next   = NULL;

	before_from = DlistPrev(from);
	before_to   = DlistPrev(to);
	dest_next   = DlistNext(dest);

	if(IsSameIter(from, to))
	{
		return ;
	}
	((node_t *)before_from)->next = (node_t *)to;
	((node_t *)to)->prev = (node_t *)before_from;

	((node_t *)before_to)->next = (node_t *)dest_next;
	((node_t *)dest_next)->prev = (node_t *)before_to;

	((node_t *)dest)->next = (node_t *)from;
	((node_t *)from)->prev = (node_t *)dest;
}

/*do the compare function from "from" until "to" (not neccesseraly all the list)
include "from" but exclude "to", if not found - return "to" . O(n)*/
iter_t DlistFind(iter_t from, iter_t to, is_match_func_t is_match, const void *param)
{
	while (!(IsSameIter(from, to)))
	{	
		if (1 == is_match(DlistGetData(from), param))
		{
			return from;
		}

		from = DlistNext(from);
	}

	return to;
}

/*do the action function from "from" until "to" (not neccesseraly all the list)
  returns the exit status of the act_func. O(n)*/
int DlistForEach(iter_t from, iter_t to, act_func_t act, void *param)
{
	int res = 0;

	while (!(IsSameIter(from, to)))
	{	
		if (0 != (res = act(DlistGetData(from), param)))
		{
			return res;
		}
		
		from = DlistNext(from);
	}

	return 0;	
}