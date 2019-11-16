#ifndef OL61_PRI_QUEUE
#define OL61_PRI_QUEUE

#include <stddef.h> /* for size_t */

typedef int(*pq_is_before_func)(const void *data1, const void *data2, void *param);

typedef int(*pq_is_match_func)(const void *data1, const void *data2);

typedef struct p_queue_t p_queue_t; 

/*create priority queue*/
p_queue_t *PQCreate(pq_is_before_func is_before, void *param);
/*free the sorted list and administrate struct*/
void PQDestroy(p_queue_t *pqueue);
/*returned the data of the first node*/
void *PQueuePeek(const p_queue_t *pqueue);
/*erase the last node, and returned is data*/
void *PQDequeue(p_queue_t *pqueue);
/*add new node, returned if seccess or not*/
int PQEnqueue(p_queue_t *pqueue , void *data);
/*returned the size of the queue*/
size_t PQSize(const p_queue_t *pqueue);
/*returened if empty or not*/
int PQIsEmpty(const p_queue_t *pqueue);
/*merge tow sorted queues*/
void PQMerge(p_queue_t *from, p_queue_t *to);
/*erase the required node, and returned the data of the erased node*/
void *PQErase(p_queue_t *pqueue, pq_is_match_func is_match, void *param);
/*erased all queue*/
void PQClear(p_queue_t *pqueue);

#endif /*OL61_PRI_QUEUE*/