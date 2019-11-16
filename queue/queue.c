#include <assert.h>      /* assert */
#include <stdlib.h>      /* allocation */
#include "queue.h"       /* my header */
#include "linked_list.h" /* linked list */

/* queue struct */
struct queue_t
{
	slist_node_t *head;
	slist_node_t *tail;
};

/* allocate memory for new queue, dont get anything and will create a dummy */
queue_t *QueueCreate(void)
{
	slist_node_t *dummy = NULL;
	queue_t *q          = NULL;

	q = malloc(sizeof(queue_t));
	if (!q)
	{
		return NULL;
	}

	dummy = SlistCreateNode(NULL, NULL);
	if (!dummy)
	{
		free(q);
		q = NULL;
		return NULL;
	}

	q->head = dummy;
	q->tail = dummy;

	return q;
}

/* free all allocated memory */
void QueueDestroy(queue_t *queue)
{
	assert(queue);

	SlistFreeAll(queue->head);
	free(queue);
	queue = NULL;
}

/* add new element in the queue, 0 - success 1 - fail, O(1) */
int QueueEnqueue(queue_t *queue, void *data)
{
	slist_node_t *temp_node = NULL;

	temp_node = SlistCreateNode(data, NULL);
	if (!temp_node)
	{
		return 1;
	}

	SlistInsert(queue->tail, temp_node);
	queue->tail = queue->tail->next;

	return 0;
}

/* remove element from the queue, return dequeued data , O(1) */
void *QueueDequeue(queue_t *queue)
{
	void *deq_val       = NULL;
	slist_node_t *tmp_q = queue->head;

	assert(queue);

	deq_val = QueuePeek(queue);
	queue->head = queue->head->next;

	free(tmp_q);
	tmp_q = NULL;

	return deq_val;
}

/* peek, return the first element in queue without changing it O(1) */
void *QueuePeek(queue_t *queue)
{	
	assert(queue);
	assert(!QueueIsEmpty(queue));

	return queue->head->data;
}

/* return the queue size O(N)*/
size_t QueueSize(const queue_t *queue)
{
	size_t count = 0;

	assert(queue);

	count = SlistCount(queue->head);

	return (count - 1);
}

/* check if queue is empty , 1 - true 0 - false O(1)*/
int QueueIsEmpty(const queue_t *queue)
{
	assert(queue);

	if  (!queue->head->next)
	{
		return 1;
	}

	return 0;
}

/* merge 2 queues */
queue_t *QueueAppend(queue_t *to, queue_t *from)
{
	assert(to);
	assert(from);

	to->tail->next = from->head->next;
	to->tail->data = from->head->data;
	to->tail = from->tail;

	/* destory from struct */
	from->tail = NULL;
	from->head->next = NULL;
	QueueDestroy(from);

	return to;
}