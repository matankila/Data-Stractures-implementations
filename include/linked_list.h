#ifndef OL61_LINKED_LIST
#define OL61_LINKED_LIST

#include <stddef.h> /* for size_t */

typedef struct node slist_node_t;

struct node{
	void *data;
	slist_node_t *next;
};

/*a pointer to compare function - for SlistFind*/
typedef int (*cmp_func_t)(const void *data, const void *param);

/*a pointer to action function - for SlistForEach*/
typedef int (*act_func_t)(void *data, void *param);

/*return pointer to the new node*/
slist_node_t *SlistCreateNode(void *data, slist_node_t *next);

/*free all the list*/
void SlistFreeAll(slist_node_t *head);

/*return pointer to the new node*/ /*may cause invalidation of user's pointers*/
slist_node_t *SlistInsert(slist_node_t *where, slist_node_t *new_node);  

/*return pointer to the new node*/
slist_node_t *SlistInsertAfter(slist_node_t *where, slist_node_t *new_node);

/*assert if last*/ /*return pointer to the node after the deleted node */
slist_node_t *SlistRemove(slist_node_t *delete_node);   

/*return pointer to the node before the deleted node */
slist_node_t *SlistRemoveAfter(slist_node_t *where); 

/*return pointer to a node*/
slist_node_t *SlistFind(slist_node_t *head, cmp_func_t cmp, const void *param);

/*return 0 - success, 1 - failure */
int SlistForEach(slist_node_t *head, act_func_t act, void *param);

/*return count of nodes*/
size_t SlistCount(const slist_node_t *head);

/*boolean. return 1 - true, 0 - false */
int SlistHasLoop(const slist_node_t *head);  

/*return 0 - success, 1 - failure */
int SlistResolveLoop(slist_node_t *head);  

/*return pointer to the intersection*/
const slist_node_t *SlistFindIntersection(const slist_node_t *head1, 
	                                      const slist_node_t *head2);   

/*return 0 - success, 1 - failure */
int SlistResolveIntersection(slist_node_t *head1, slist_node_t *head2);  

/*return pointer to the new first node*/ /*can be recursive*/
slist_node_t *SlistFlip(slist_node_t *head); 

slist_node_t *SlistRecFlip(slist_node_t *head); 

#endif