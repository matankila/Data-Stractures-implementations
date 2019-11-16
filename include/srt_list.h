#ifndef OL61_SRT_LIST
#define OL61_SRT_LIST

#include <stddef.h> /*size_t*/

/* is before, fucntion for the sort method to use */
typedef int (*srt_is_before_func_t)(const void *data1, const void *data2, void *param);

/* fucnction for matching data. 1 - true, 0 - false */
typedef int (*srt_is_match_func_t)(const void *data1, const void *data2);

/* fucntion that doing an action on the list. 0 - success , 1 - fail */
typedef int (*srt_act_func_t)(void *data, void *param);

typedef struct srt_list_t srt_list_t;

typedef struct srt_list_iter_t
{ 
	struct srt_list_info *info;
#ifndef NDEBUG
	size_t list_id;
#endif
}srt_list_iter_t;

/*
* create a sorted list with two dummies - 
* head & tail, and uniqe number to each O(1) 
*/
srt_list_t *SrtlistCreate(srt_is_before_func_t is_before, void *param);

/* free all allocated memory O(n) */
void SrtlistDestroy(srt_list_t *srt_list);

/* inserted iter in success, end of list in failure. O(n) */
srt_list_iter_t SrtlistInsert(srt_list_t *srt_list, void *data);

/*
* erase the node in the iter address.
* returns the next iter in the list. O(1)
*/
srt_list_iter_t SrtlistErase(srt_list_iter_t where);

srt_list_iter_t SrtlistFind(srt_list_t *srt_list, const void *data);

/*do the action function from "from" until "to" (not neccesseraly all the list)
  returns the exit status of the act_func. O(n)*/
int SrtlistForEach(srt_list_iter_t from,
                   srt_list_iter_t to,
                   srt_act_func_t act,
                   void *act_param);

/* return list size, O(n) */
size_t SrtlistSize(const srt_list_t *srt_list);

/* boolean return value - 1 ture, 0 false. O(1) */
int SrtlistIsEmpty(const srt_list_t *srt_list);

/* returns the data of the poped node. O(1) */
void *SrtlistPopFront(srt_list_t *srt_list);

/* returns the data of the poped node. O(1) */
void *SrtlistPopBack(srt_list_t *srt_list);

/*
* returns the first valid node in the list. (after the dummy head)
* if the list is empty lets the user know (end-of-list iter). O(1) 
*/
srt_list_iter_t SrtlistBegin(srt_list_t *srt_list);

/* o(1) non-valid element */
srt_list_iter_t SrtlistEnd(srt_list_t *srt_list);

/* o(1) */
srt_list_iter_t SrtlistNext(srt_list_iter_t);

/* o(1) */
srt_list_iter_t SrtlistPrev(srt_list_iter_t);

/*
* boolean return value - 1 true, 0 fail o(1)
* check if iter1 and iter2 are the same  
*/
int SrtlistIsSameIter(srt_list_iter_t iter1, srt_list_iter_t iter2);

/* get data by iter o(1) */
void *SrtlistGetData(srt_list_iter_t);

/* compare not by main compare field */
srt_list_iter_t SrtlistFindIf(srt_list_iter_t from,
							  srt_list_iter_t to, 
							  srt_is_match_func_t is_match,
							  const void *data);

/*
* merge 2 sorted lists to one sorted list by to.
* uses splice
*/
void SrtlistMerge(srt_list_t *from, srt_list_t *to);

#endif /* OL_61_SRT_LIST */
