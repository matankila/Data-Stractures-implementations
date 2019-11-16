#include <stdio.h>  /* IO fucntion */ 
#include "bstree.h" /* my header   */

int err   = 0;
int tests = 0;

int Cmp(const void *data1, const void *data2, void *param);
int Act(void *data, void *param);
void CasualTest();

int main()
{
	CasualTest();

	if(err > 0)
	{
		printf("found %d errors \n", err);
	}
	else
	{
		printf("passed %d tests successfuly !!!\n", tests);
	}
	
	return (0);
}

int Cmp(const void *data1, const void *data2, void *param)
{
	if(*(int *)data1 < *(int *)data2)
	{
		return 1;
	}

	return 0;
}

int Act(void *data, void *param)
{
	printf("%d, ", *(int *)data);

	return 0;
}

void CasualTest()
{
	int elanor = 26;
	int matan  = 22;
	int ofir   = 29;
	bst_iter from      = NULL;
	bst_iter to        = NULL;
	bs_tree_t *tree    = NULL;
	bst_iter to_remove = NULL;

	tree = BstCreate(Cmp, NULL);
	if(!tree)
	{
		++err;
		printf("problem with create  line %d\n", __LINE__);
	}
	to_remove = BstInsert(tree, &elanor);
	BstInsert(tree, &ofir);
	BstInsert(tree, &matan);
	from = BstBegin(tree);
	to   = BstEnd(tree);
	if(3 != BstCount(tree))
	{
		++err;
		printf("%lu\n", BstCount(tree));
		printf("problem with count/insert line %d\n", __LINE__);
	}

	BstRemove(to_remove);

	if(22 != *(int*)BstGetData(BstFind(tree, &matan)))
	{
		++err;
		printf("problem with find line %d\n", __LINE__);
	}
	from = BstBegin(tree);
	to   = BstEnd(tree);
	if(2 != BstCount(tree))
	{
		++err;
		printf("problem with count/insert/remove line %d %d\n", __LINE__);
	}

	BstDestroy(tree);
	tests += 4;
}