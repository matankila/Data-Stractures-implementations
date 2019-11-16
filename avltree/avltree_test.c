#include <stdio.h>   /* for i/o functions          */
#include "avltree.h" /* for functions declerations */

#define SMALLER -1
#define BIGGER 1
#define EQUAL 0
#define SIZE 18

size_t err_count = 0;

int DataCompare(void const *data1, void const *data2, void *const param);
void AVLTreeTest();

int main()
{
	  AVLTreeTest();

	  if (!err_count)
	  {
		  printf("\n\33[1;32mAll tests were successful!\33[0;0m\n\n");
	  }
    else
    {
      printf("review your errors maybe its about my compare fucntion\n");
      printf("please check it..\n");
      printf("to see if your balncing works use print of balance factor\n");
      printf("in your size fucntion (it works after all insert and after\n");
      printf("delete to..\n");
    }

    return 0;
}

int DataCompare(void const *data1, void const *data2, void *const param)
{
    (void)param;

    return (*(int *)data1 - *(int *)data2);
}

int act(void *internal_data, void *param)
{
    (void)param;

    printf("%d\n", *(int *)internal_data);

    return 0;
}

void AVLTreeTest()
{
    int i = 0;
    int data_to_insert[SIZE] = {20,3,6,14,30,7,23,2,1,13,4,12,24,11,9,18,15,21};
    int inserted_data = 0;
    avl_tree_t *tree = AVLCreate(DataCompare, NULL);

    if (!tree)
    {
      printf("\33[1;31mBstCreate failed in line %d\33[0;0m\n", __LINE__);
      ++err_count;
    }

    for (i = 0; i < SIZE; ++i)
    {
      inserted_data = AVLInsert(tree, &data_to_insert[i]);
      if (0 != inserted_data)
      {
        printf("\33[1;31minserted data not as expected due to line %d\33[0;0m\n", __LINE__);
        ++err_count;
      }
    }

    if (AVLSize(tree) != SIZE)
    {
      printf("\33[1;31mBstCreate failed in line %d\33[0;0m\n", __LINE__);
      ++err_count;
    }

    if (AVLIsEmpty(tree))
    {
      printf("\33[1;31mBstCreate failed in line %d\33[0;0m\n", __LINE__);
      ++err_count;
    }
    if (5 != AVLHeight(tree))
    {
      printf("\33[1;31mBstCreate failed in line %d\33[0;0m\n", __LINE__);
      ++err_count;
    }

    printf("the size is:%ld\n", AVLSize(tree));
    printf("%d\n", AVLForEach(tree, act, NULL));

    AVLRemove(tree, &data_to_insert[4]);
    printf("\n");

    printf("the size is:%ld\n", AVLSize(tree));
    printf("%d\n", AVLForEach(tree, act, NULL));   

    AVLDestroy(tree);
}