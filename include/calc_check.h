/*********************
* Matan Keler        *
* OL61               * 
* check the exp      *
*********************/

#ifndef OL61_CALC_CHECK
#define OL61_CALC_CHECK

/*
* Arguments: string with mathmatical infix exp
* Return Value: 
* 1, True if parenthasis are good
* 0, False if not
*/
int IsGoodParInExpression(char *str);

/*
* Arguments: string with mathmatical infix exp
* Return Value: 
* 1, True if the expression is valid
* 0, False if not
*/
int IsGoodExpression(char *str);

#endif /* OL61_CALC_CHECK */