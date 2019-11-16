/*********************
* Matan Keler        *
* OL61               * 
* calc the postfix   *
*********************/

#ifndef OL61_CALC_POSTFIX
#define OL61_CALC_POSTFIX
#include "stack.h"

typedef double (*op_func)(double n1, double n2);

/*
* calculate hard postfixe expression
* arguments: stack - contain the postfixed expression
* return: calculation of the value
* NOTE: in failure the function will print err to stderr & return 0xDEADBEEF
*/
double CalcSimplePostfix(char *postfixed);

/*
* calculate hard postfixe expression
* arguments: stack - contain the postfixed expression
* return: calculation of the value
* NOTE: in failure the function will print err to stderr & return 0xDEADBEEF
*/
double CalacExpression(Stack_t *stack);

#endif /* OL61_CALC_POSTFIX */