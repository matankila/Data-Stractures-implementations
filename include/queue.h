/***************************************/
/*        Queue ex, 04/12/18           */
/***************************************/ 

#ifndef OL61_QUEUE
#define OL61_QUEUE

#include <stddef.h>   /* for size_t */
#include "srt_list.h" /* my header */

/* struct of queue */
typedef struct queue_t queue_t; 

/* allocate memory for new queue, dont get anything and will create a dummy */
queue_t *QueueCreate(void);

/* free all allocated memory */
void QueueDestroy(queue_t *queue);

/* add new element in the queue, 0 - success 1 - fail, O(1) */
int QueueEnqueue(queue_t *queue, void *data);

/* remove element from the queue, return dequeued data , O(1) */
void *QueueDequeue(queue_t *queue);

/* peek, return the first element in queue without changing it O(1) */
void *QueuePeek(queue_t *queue);

/* return the queue size O(N)*/
size_t QueueSize(const queue_t *queue);

/* check if queue is empty , 1 - true 0 - false O(1)*/
int QueueIsEmpty(const queue_t *queue);

/* merge 2 queues */
queue_t *QueueAppend(queue_t *to, queue_t *from);

#endif