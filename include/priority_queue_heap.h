#ifndef OL61_PRIORITY_QUEUE_HEAP
#define OL61_PRIORITY_QUEUE_HEAP

#include <stddef.h> /* for size_t */
#include "heap.h"   /* my header */

typedef int (*pq_comp_func_t)(const void *internal_data,
								const void *external_data,
								void *param);
typedef unsigned int (*pq_is_match_func_t)(void *const data, void *const param);

typedef struct p_queue_t p_queue_t; 

/*create priority queue*/
p_queue_t *PQCreate(size_t num_of_elements, pq_comp_func_t compare,
                    void *param);

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

/*erase the required node, and returned the data of the erased node*/
void *PQErase(p_queue_t *pqueue, pq_is_match_func_t is_match,
              const void *param, const void *data);

/*erased all queue*/
void PQClear(p_queue_t *pqueue);

#endif /* OL61_PRIORITY_QUEUE_HEAP */