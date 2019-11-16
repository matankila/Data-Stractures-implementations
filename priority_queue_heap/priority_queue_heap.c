#include <assert.h>              /* assert      */
#include <assert.h>              /* assert      */
#include <stdlib.h>              /* allocation  */
#include <string.h>              /* memset      */
#include "heap.h"                /* my header   */
#include "priority_queue_heap.h" /* my header   */

struct p_queue_t
{
	void **p_queue;
	heap_t *heap;
};

/*create priority queue*/
p_queue_t *PQCreate(size_t num_of_elements, pq_comp_func_t compare,
                    void *param)
{
	p_queue_t *pq = NULL;

	assert(compare);

	pq = malloc(sizeof(p_queue_t));
	if (NULL == pq)
	{
		return NULL;
	}

	pq->p_queue = malloc(sizeof(void *) * num_of_elements);
	if (NULL == pq->p_queue)
	{
		free(pq);
		pq = NULL;

		return NULL;
	}
	memset(pq->p_queue, 0, sizeof(void *) * num_of_elements);

	pq->heap = malloc(HeapGetSizeToAlloc());
	if (NULL == pq->heap)
	{
		free(pq->p_queue);
		pq->p_queue = NULL;
		free(pq);
		pq =  NULL;

		return NULL;
	}

	HeapInit(pq->heap, pq->p_queue, num_of_elements, compare, param);

	return pq;
}

/*free the sorted list and administrate struct*/
void PQDestroy(p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	free(pqueue->heap);
	pqueue->heap = NULL;
	free(pqueue->p_queue);
	pqueue->p_queue = NULL;
	free(pqueue);
	pqueue = NULL;
}

/*returned the data of the highest priority node*/
void *PQueuePeek(const p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	return (HeapPeek(pqueue->heap));
}

/*erase the last node, and returned its data*/
void *PQDequeue(p_queue_t *pqueue)
{
	void *return_val = NULL;

	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	return_val = PQueuePeek(pqueue);
	HeapPop(pqueue->heap);

	return (return_val);	
}

/*add new node, returned 0 if seccess 1 if not*/
int PQEnqueue(p_queue_t *pqueue , void *data)
{
	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	return (HeapPush(pqueue->heap, data));
}

/*returned the size of the queue*/
size_t PQSize(const p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	return (HeapSize(pqueue->heap));
}

/*returened if empty or not*/
int PQIsEmpty(const p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	return (HeapIsEmpty(pqueue->heap));
}

/*erase the required node, and returned the data of the erased node*/
void *PQErase(p_queue_t *pqueue, pq_is_match_func_t is_match,
              const void *param, const void *data)
{
	void *erased_val  = NULL;

	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	erased_val = HeapErase(pqueue->heap, data, is_match, param);

	return erased_val;
}

/*erased all queue*/
void PQClear(p_queue_t *pqueue)
{
	assert(pqueue);
	assert(pqueue->p_queue);
	assert(pqueue->heap);

	while (!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}
