#include <stdio.h>               /* IO fucntion */ 
#include "priority_queue_heap.h" /* my header   */

#define Init(pq, arr, len, i) for(i = 0; i < len; i++) \
                              { \
                              	if(PQEnqueue(pq, &arr[i]))\
                              	{ \
                              		++err_count; \
                              		printf("failed enqueue\n"); \
                              	} \
                              }

#define Destroy(pq) PQDestroy(pq); \
                    pq = NULL; \

int err_count  = 0;
int test_count = 0;
p_queue_t *pt = NULL;

/****************************** priority function ****************************/
int is_p(const void *data1, const void *data2, void *param);
unsigned int mach(void *const data1, void *const data2);

/******************************** test function ******************************/

/*
* check all regular fucntions
* test: create, enqueue, destroy, isempty, size 
*/
void CasualTest();

/*
* check erase fucntion
* test: create, enqueue, destroy, isempty, size, erase 
*/
void EraseTest();

/*
* check erase fucntion
* test: create, enqueue, destroy, isempty, size, clear 
*/
void ClearTest();

int main()
{
	CasualTest();
	EraseTest();
	ClearTest();

	if(err_count > 0)
	{
		printf("%d / %d failed \n",err_count, test_count);
	}
	else
	{
		printf("SUCCESS OVER 9000!!!!\n");
	}

	return (0);
}

/****************************** priority function ****************************/
int is_p(const void *internal_data, const void *external_data, void *param)
{
	(void)param;

	return (*(int *)external_data - *(int *)internal_data);
}

unsigned int mach(void *const data1, void *const data2)
{
	if(*(int *)data1 == *(int *)(((ret_data_t *)data2)->data))
	{
		return 1;
	}

	return 0;
}

/******************************** test function ******************************/

/*
* check all regular fucntions
* test: create, enqueue, destroy, isempty, size 
*/
void CasualTest()
{
	p_queue_t *p = NULL;
	int arr[5]   = {1, 4, 5, 2, 3};
	int i        = 0;
	int len      = 5;

	p = PQCreate(len, is_p, NULL);
	Init(p, arr, len, i); /* look line 5, enqueue 5 elements */

	/* 
	* after create and init we have pq with 5 elements
	* test isempty and size 
	*/
	if(PQIsEmpty(p))
	{
		++err_count;
		printf("failed isempty %d\n", __LINE__);
	}
	if(5 != PQSize(p))
	{
		++err_count;
		printf("failed size %d\n", __LINE__);
	}

	test_count += 4;

	Destroy(p); /* look line 6, destroy the pq */
	p = NULL;
}

/*
* check erase fucntion
* test: create, enqueue, destroy, isempty, size, erase 
*/
void EraseTest()
{
	p_queue_t *p = NULL;
	int arr[5]   = {1, 4, 5, 2, 3};
	int i        = 0;
	int len      = 5;
	void *ret    = NULL;

	p = PQCreate(len, is_p, NULL);
	Init(p, arr, len, i); /* look line 5, enqueue 5 elements */
	ret = PQErase(p, mach, NULL, &arr[1]);
	if(4 != *(int *)ret)
	{
		printf("failed erase %d\n", __LINE__);
		printf("%d\n",  *(int *)ret);
	}

	/* 
	* after create and init we have pq with 5 elements
	* test isempty and size 
	*/
	if(PQIsEmpty(p))
	{
		++err_count;
		printf("failed isempty %d\n", __LINE__);
	}

	if(4 != PQSize(p))
	{
		++err_count;
		printf("failed size %d\n", __LINE__);
	}

	test_count += 4;

	Destroy(p); /* look line 6, destroy the pq */
	p = NULL;
}

/*
* check erase fucntion
* test: create, enqueue, destroy, isempty, size, clear 
*/
void ClearTest()
{
	p_queue_t *p = NULL;
	int arr[5]   = {1, 4, 5, 2, 3};
	int i        = 0;
	int len      = 5;

	p = PQCreate(len, is_p, NULL);
	Init(p, arr, len, i); /* look line 5, enqueue 5 elements */

	PQClear(p);

	/* 
	* after create and init we have pq with 5 elements
	* test isempty and size 
	*/
	printf("%ld\n", PQSize(p));
	if(!PQIsEmpty(p))
	{
		++err_count;
		printf("failed isempty %d\n", __LINE__);
	}

	if(0 != PQSize(p))
	{
		++err_count;
		printf("failed size %d\n", __LINE__);
	}

	test_count += 4;

	Destroy(p); /* look line 6, destroy the pq */
	p = NULL;
}