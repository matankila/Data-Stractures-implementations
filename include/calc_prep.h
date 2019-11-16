/*********************
* Matan Keler        *
* OL61               * 
* Prepare to calc    *
*********************/

#ifndef OL61_CALC_PREP
#define OL61_CALC_PREP

#include "stack.h" /* stack */

/*
* Arguments: string, contain an infix mathmatical expression
* Return Value: Stack, constain the same expression in postifx
*/
Stack_t *CreatePostFix(char *str);

/*
* Arguments: stack with postifx expression
* Return value: none, destory the stack
*/
void DestroyPostFix(Stack_t *stack);

#endif /* OL61_CALC_PREP */