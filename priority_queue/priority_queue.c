#include <stdio.h>          /* IO fucntion */
#include <assert.h>         /* assert      */
#include <stdlib.h>         /* allocation  */
#include "srt_list.h"       /* my header   */
#include "priority_queue.h" /* my header   */

struct p_queue_t
{
	srt_list_t *p_queue;
};

/*create priority queue*/
p_queue_t *PQCreate(pq_is_before_func is_before, void *param)
{
	p_queue_t *pq = NULL;

	assert(is_before);

	pq = malloc(sizeof(p_queue_t));
	if (pq)
	{
		pq->p_queue = SrtlistCreate(is_before, param);
		if (!pq->p_queue)
		{
			free(pq);
			pq = NULL;
		}
	}

	return pq;
}

/*free the sorted list and administrate struct*/
void PQDestroy(p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	SrtlistDestroy(pqueue->p_queue);
	pqueue->p_queue = NULL;

	free(pqueue);
	pqueue = NULL;
}

/*returned the data of the highest priority node*/
void *PQueuePeek(const p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	return (SrtlistGetData(SrtlistBegin(pqueue->p_queue)));
}

/*erase the last node, and returned its data*/
void *PQDequeue(p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	return (SrtlistPopFront(pqueue->p_queue));	
}

/*add new node, returned 0 if seccess 1 if not*/
int PQEnqueue(p_queue_t *pqueue , void *data)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	return (SrtlistIsSameIter(SrtlistInsert(pqueue->p_queue, data),
	                      SrtlistEnd(pqueue->p_queue)));
}

/*returned the size of the queue*/
size_t PQSize(const p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	return (SrtlistSize(pqueue->p_queue));
}

/*returened if empty or not*/
int PQIsEmpty(const p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	return (SrtlistIsEmpty(pqueue->p_queue));
}

/*merge tow sorted queues*/
void PQMerge(p_queue_t *from, p_queue_t *to)
{
	assert(from);
	assert(from->p_queue);
	assert(to);
	assert(to->p_queue);

	SrtlistMerge(from->p_queue, to->p_queue);

	PQDestroy(from);
	from = NULL;
}

/*erase the required node, and returned the data of the erased node*/
void *PQErase(p_queue_t *pqueue, pq_is_match_func is_match, void *param)
{
	srt_list_iter_t erased_node = {NULL};
	void *          erased_val  = NULL;

	assert(pqueue);
	assert(pqueue->p_queue);

	erased_node = SrtlistFindIf(SrtlistBegin(pqueue->p_queue),
		          SrtlistEnd(pqueue->p_queue), is_match, param);
	if(SrtlistIsSameIter(erased_node,SrtlistEnd(pqueue->p_queue)))
	{
		return NULL;
	}

	erased_val  = SrtlistGetData(erased_node);

	SrtlistErase(erased_node);

	return erased_val;
}

/*erased all queue*/
void PQClear(p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);

	while (!SrtlistIsEmpty(pqueue->p_queue))
	{
		SrtlistPopBack(pqueue->p_queue);
	}
}