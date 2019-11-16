#include <stdio.h>       /* IO fucntion       */
#include <assert.h>      /* assert            */
#include <stdlib.h>      /* memory allocation */
#include "linked_list.h" /* my header         */

static void swap(void **ptr1, void **ptr2);

/* private fucntion that swap pointers of any kind */
static void swap(void **ptr1, void **ptr2)
{
	void *temp = *ptr1;

	*ptr1 = *ptr2;
	*ptr2 = temp;
}

/*return pointer to the new node*/
slist_node_t *SlistCreateNode(void *data, slist_node_t *next)
{
	slist_node_t *head = malloc(sizeof(slist_node_t));
	if(!head)
	{
		return NULL;
	}

	head->data = data;
	head->next = next;

	return head;
}

/*free all the list*/
void SlistFreeAll(slist_node_t *head)
{
	slist_node_t *free_ptr = head;

	assert(head);

	while (free_ptr)
	{
		head = head->next;
		free(free_ptr);
		free_ptr = head;
	}
}

/*return pointer to the new node*/ /*may cause invalidation of user's pointers*/
slist_node_t *SlistInsert(slist_node_t *where, slist_node_t *new_node)
{
	assert(where);
	assert(new_node);

	SlistInsertAfter(where, new_node);

	swap((void *)&where->data, (void *)&new_node->data);

	return where;
}  

/*return pointer to the new node*/
slist_node_t *SlistInsertAfter(slist_node_t *where, slist_node_t *new_node)
{
	assert(where);
	assert(new_node);

	new_node->next = where->next;
	where->next    = new_node;

	return where->next;
}

/*assert if last return pointer to the node after the deleted node */
slist_node_t *SlistRemove(slist_node_t *delete_node)
{
	assert(delete_node->next);

	swap((void *)&delete_node->data,(void *)&delete_node->next->data);
	SlistRemoveAfter(delete_node);

	return delete_node;
}   

/*return pointer to the node before the deleted node */
slist_node_t *SlistRemoveAfter(slist_node_t *where)
{
	slist_node_t *free_ptr = where;

	assert(where);

	free_ptr    = where->next;
	where->next = where->next->next;

	free(free_ptr);
	free_ptr = NULL;

	return where;
} 

/*return pointer to a node*/
slist_node_t *SlistFind(slist_node_t *head, cmp_func_t cmp, const void *param)
{
	assert(head);
	assert(param);
	assert(cmp);

	/* for each element use cmp fucntion until found element */
	while (head && 0 == cmp(head->data, param))
	{	
		head = head->next;
	}

	if (!head)
	{
		return NULL;
	}

	return head;
}

/*return 0 - success, 1 - failure */
int SlistForEach(slist_node_t *head, act_func_t act, void *param)
{
	assert(head);
	assert(act);

	/* for each element use action fucntion */
	while (head)
	{
		if (1 == act(head->data, param))
		{
			return 1; /* fail */
		}

		head = head->next;
	}

	return 0;
}

/*return count of nodes*/
size_t SlistCount(const slist_node_t *head)
{
	size_t count = 0;

	assert(head);

	while (head)
	{
		++count;

		head = head->next;
	}

	return count;
}

/*boolean. return 1 - true, 0 - false */
int SlistHasLoop(const slist_node_t *head)
{
	const slist_node_t *fast = head->next, *slow = head;
	int return_val = 1;

	assert(head);	

	/* if the slower pointer chatch up to the faster we have a loop */
	while (fast != slow)
	{
		if (!fast || !slow || !(fast->next))
		{
			return_val = 0; /* false */
			break;
		}

		fast = fast->next->next;
		slow = slow->next;
	}

	return return_val;
}  

/*return 0 - success, 1 - failure */
int SlistResolveLoop(slist_node_t *head)
{
	slist_node_t *fast = head->next;
	slist_node_t *slow = head;

	assert(head);

	/* found the point where fast and slow meet on loop */
	while (fast != slow)
	{
		if (!fast || !slow || !(fast->next))
		{
			return 1; /* fail */
		}

		fast = fast->next->next;
		slow = slow->next;
	}
	/* inc fast and break the chain */
	fast       = fast->next;
	slow->next = NULL;

	/* if after breaking the chain theres no loop */
	if(0 == SlistHasLoop(head))
	{
		return 0; /* success */
	}

	/* with 2 chains with intersection resolve intersection */
	if(1 == SlistResolveIntersection(head, fast))
	{
		return 1; /* fail */
	}

	/* reconnect the chains */
	slow->next = fast;

	return 0; /* success */
}  

/*return pointer to the intersection*/
const slist_node_t *SlistFindIntersection(const slist_node_t *head1, const slist_node_t *head2)
{
	const slist_node_t *bigger  = head1;
	const slist_node_t *smaller = head2;
	size_t count_h1             = 0;
	size_t count_h2             = 0;
	int diff                    = 0;

	assert(head1);
	assert(head2);

	count_h1 = SlistCount(head1);
	count_h2 = SlistCount(head2);
	diff     = count_h1 - count_h2;
	
	/* if opposit value than of asumption swap it */
	if (count_h2 > count_h1)
	{
		diff = count_h2 - count_h1;
		swap((void *)&bigger, (void *)&smaller);
	}
	/* inc the bigger until its on same length as smaller */
	while (diff)
	{
		bigger = bigger->next;
		--diff;
	}
	/* if they are equal intersection found !*/
	while (bigger && smaller && bigger != smaller)
	{
		bigger  = bigger->next;
		smaller = smaller->next;
	}

	return bigger;
}   

/*return 0 - success, 1 - failure */
int SlistResolveIntersection(slist_node_t *head1, slist_node_t *head2)
{
	const slist_node_t *inter_pt = head1;

	assert(head1);
	assert(head2);

	/* get intersection point to later check, if theres no intersection return 1 */
	inter_pt = SlistFindIntersection(head1, head2);
	if (!inter_pt)
	{
	    return 1;
	}

	/* if equal to intersection point cut chain and return 0 */
	while (head2 != inter_pt)
	{
		head2 = head2->next;
	}

	head2->next = NULL;

	return 0;
}  

/*return pointer to the new first node*/ /*can be recursive*/
slist_node_t *SlistFlip(slist_node_t *head)
{
	slist_node_t *current  = NULL;
    slist_node_t *previous = NULL;

    assert(head);

    /* link head->next to prevoius element while saving the next element */
    while (head) 
    {
        current    = head->next;
        head->next = previous;
        previous   = head;
        head       = current;
    }

	return previous;
} 

static slist_node_t *RecFlip(slist_node_t *current, slist_node_t *rev_next)
{
	slist_node_t *real_next = NULL;

	assert(current);

	real_next = current->next;

	if(NULL == real_next)
	{
		current->next = rev_next;

		return current;
	}

	current->next = rev_next;

	return (RecFlip(real_next, current));
}

slist_node_t *SlistRecFlip(slist_node_t *head)
{
	assert(head);

	return (RecFlip(head, NULL));
}