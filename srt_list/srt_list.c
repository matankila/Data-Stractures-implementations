#include <assert.h>   /* assert    */
#include <stdlib.h>   /* malloc    */
#include "dl_list.h"  /* dlist     */
#include "srt_list.h" /* my header */

struct srt_list_t
{
	dlist_t *dlist;
#ifndef NDEBUG
	size_t list_id;
#endif
	void *param;
	int(*is_before)(const void *data1, const void *data2, void *param);
};

typedef struct srt_list_info info_t;
static srt_list_iter_t SrtlistFindAfter(srt_list_t *srt_list,
                                         const void *data);

/*
* create a sorted list with two dummies - 
* head & tail, and uniqe number to each O(1) 
*/
srt_list_t *SrtlistCreate(srt_is_before_func_t is_before, void *param)
{
	srt_list_t *ptr = NULL;

	assert(is_before);

	ptr = malloc(sizeof(srt_list_t));
	if (!ptr)
	{
		return NULL;
	}

	ptr->dlist = DlistCreateList();
	if (!(ptr->dlist))
	{
		free(ptr);
		ptr = NULL;

		return NULL;
	}
#ifndef NDEBUG
	ptr->list_id   = (size_t)&ptr;
#endif
	ptr->param     = param;
	ptr->is_before = is_before;

	return ptr;
}

/* free all allocated memory O(n) */
void SrtlistDestroy(srt_list_t *srt_list)
{
	assert(srt_list);
	assert(srt_list->dlist);

	DlistDestroy(srt_list->dlist);
	srt_list->dlist = NULL;

	free(srt_list);
	srt_list = NULL;
}

/* inserted iter in success, end of list in failure. O(n) */
srt_list_iter_t SrtlistInsert(srt_list_t *srt_list, void *data)
{	
	srt_list_iter_t ret = {NULL};

	assert(srt_list);
	assert(srt_list->dlist);

	ret      = SrtlistFindAfter(srt_list, data);
	ret.info = (info_t *)DlistInsert(srt_list->dlist, (iter_t)ret.info, data);
	#ifndef NDEBUG	
		ret.list_id = srt_list->list_id;
	#endif

	return ret;
}

/*
* erase the node in the iter address.
* returns the next iter in the list. O(1)
*/
srt_list_iter_t SrtlistErase(srt_list_iter_t where)
{
	srt_list_iter_t next_iter = {NULL};

	assert(where.info);

	next_iter.info = (info_t *)DlistErase((iter_t)where.info);

	return next_iter;
}

srt_list_iter_t SrtlistFind(srt_list_t *srt_list, const void *data)
{
	srt_list_iter_t begin = {NULL};
	srt_list_iter_t end   = {NULL};

	assert(srt_list);
	assert(srt_list->dlist);

	(void)(end);

	begin = SrtlistBegin(srt_list);
	end   = SrtlistEnd(srt_list);

	while (!SrtlistIsSameIter(begin, end) &&
		  srt_list->is_before(data, SrtlistGetData(begin), srt_list->param) !=srt_list->is_before(SrtlistGetData(begin), data, srt_list->param))
	{
		begin = SrtlistNext(begin);
	}

	return begin;
}

/*do the action function from "from" until "to" (not neccesseraly all the list)
  returns the exit status of the act_func. O(n)*/
int SrtlistForEach(srt_list_iter_t from,
                   srt_list_iter_t to,
                   srt_act_func_t act,
                   void *act_param)
{
	assert(from.info);
	assert(to.info);
	assert(act);

	return DlistForEach((iter_t)from.info, (iter_t)to.info, act, act_param);
}

/* return list size, O(n) */
size_t SrtlistSize(const srt_list_t *srt_list)
{
	assert(srt_list);
	assert(srt_list->dlist);

	return (DlistCount(srt_list->dlist));
}

/* boolean return value - 1 ture, 0 false. O(1) */
int SrtlistIsEmpty(const srt_list_t *srt_list)
{
	assert(srt_list);
	assert(srt_list->dlist);

	return (DlistIsEmpty(srt_list->dlist));
}

/* returns the data of the poped node. O(1) */
void *SrtlistPopFront(srt_list_t *srt_list)
{
	assert(srt_list);
	assert(srt_list->dlist);

	return (DlistPopFront(srt_list->dlist));
}

/* returns the data of the poped node. O(1) */
void *SrtlistPopBack(srt_list_t *srt_list)
{
	assert(srt_list);
	assert(srt_list->dlist);

	return (DlistPopBack(srt_list->dlist));
}

/*
* returns the first valid node in the list. (after the dummy head)
* if the list is empty lets the user know (end-of-list iter). O(1) 
*/
srt_list_iter_t SrtlistBegin(srt_list_t *srt_list)
{
	srt_list_iter_t iter = {NULL};

	assert(srt_list);
	assert(srt_list->dlist);

	iter.info = (info_t *)DlistBegin(srt_list->dlist);

	#ifndef NDEBUG	
	iter.list_id = srt_list->list_id;
	#endif

	return iter;	
}

/* o(1) non-valid element */
srt_list_iter_t SrtlistEnd(srt_list_t *srt_list)
{
	srt_list_iter_t iter = {NULL};

	assert(srt_list);
	assert(srt_list->dlist);

	iter.info = (info_t *)DlistEnd(srt_list->dlist);

	#ifndef NDEBUG	
	iter.list_id = srt_list->list_id;
	#endif

	return iter;
}

/* o(1) */
srt_list_iter_t SrtlistNext(srt_list_iter_t iter)
{
	assert(iter.info);

	iter.info = (info_t *)(DlistNext((iter_t)iter.info));

	return iter;
}

/* o(1) */
srt_list_iter_t SrtlistPrev(srt_list_iter_t iter)
{
	assert(iter.info);

	iter.info = (info_t *)(DlistPrev((iter_t)iter.info));

	return iter;
}

/*
* boolean return value - 1 true, 0 fail o(1)
* check if iter1 and iter2 are the same  
*/
int SrtlistIsSameIter(srt_list_iter_t iter1, srt_list_iter_t iter2)
{
	assert(iter1.list_id == iter2.list_id);
	assert(iter1.info);
	assert(iter2.info);

	return (IsSameIter((iter_t)iter1.info, (iter_t)iter2.info));
}

/* get data by iter o(1) */
void *SrtlistGetData(srt_list_iter_t iter)
{
	assert(iter.info);

	return (DlistGetData((iter_t)iter.info));
}

/* compare not by main compare field */
srt_list_iter_t SrtlistFindIf(srt_list_iter_t from,
							  srt_list_iter_t to, 
							  srt_is_match_func_t is_match,
							  const void *data)
{
	void *from_data = NULL;

	assert(from.info);
	assert(to.info);
	assert(is_match);

	from_data = SrtlistGetData(from);

	while (!(SrtlistIsSameIter(from, to)) && !(is_match(from_data, data)))
	{
		from      = SrtlistNext(from);
		from_data = SrtlistGetData(from);
	}

	return from;
}

/*
* merge 2 sorted lists to one sorted list by to.
* uses splice
*/
void SrtlistMerge(srt_list_t *from, srt_list_t *to)
{
	srt_list_iter_t dest        = {NULL};
	srt_list_iter_t to_it       = {NULL};
	srt_list_iter_t from_it     = {NULL};
	srt_list_iter_t from_end    = {NULL};
	srt_list_iter_t from_begin  = {NULL};
	srt_list_iter_t to_begin    = {NULL};
	srt_list_iter_t to_end      = {NULL};

	assert(from);
	assert(from->dlist);
	assert(to);
	assert(to->dlist);

	(void)(to_end);

	from_end   = SrtlistEnd(from);
	from_begin = SrtlistBegin(from);
	to_begin   = SrtlistBegin(to);
	to_end     = SrtlistEnd(to);
	dest       = SrtlistPrev(to_begin);

	while (!(SrtlistIsSameIter(from_begin, from_end)))
	{
		dest    = SrtlistFindAfter(to, SrtlistGetData(from_begin));
		from_it = from_begin;
		to_it   = SrtlistFindAfter(from, SrtlistGetData(from_begin));

		DlistSplice(DlistPrev((iter_t)dest.info),
		                      (iter_t)from_it.info, 
		                      (iter_t)to_it.info); 

		if(SrtlistIsSameIter(from_it, to_it))
		{
			SrtlistInsert(to, SrtlistGetData(from_it));
			to_it = SrtlistNext(to_it);
		}

		from_begin = to_it;
	}
}

static srt_list_iter_t SrtlistFindAfter(srt_list_t *srt_list,
                                        const void *data)
{
	srt_list_iter_t begin = {NULL};
	srt_list_iter_t end   = {NULL};

	assert(srt_list);
	assert(srt_list->dlist);

	(void)(end);

	begin = SrtlistBegin(srt_list);
	end   = SrtlistEnd(srt_list);

	while (!SrtlistIsSameIter(begin, end) &&
		  !(srt_list->is_before(data, SrtlistGetData(begin), srt_list->param)))
	{
		begin = SrtlistNext(begin);
	}

	return begin;
}