#include <stdio.h> /* IO fucntion */ 
#include "fsm.h"   /* my header */

#define ACCEPT_STATE_NUM 1
#define NUM_SYMBOLS 5
#define NUM_STATES 7

enum symbols
{
	SIGN_SYMBOL,
	DIGIT_SYMBOL,
    DOT_SYMBOL,
	FINISH_SYMBOL,
	INVALID_SYMBOL
};
enum states
{
	INVALID_CHAR,
	FINISH_SUCCESFULLY,
	GOT_DOT,
	GOT_SIGN,
    GOT_DIGIT_BEFORE_DOT,
    GOT_DIGIT_AFTER_DOT,
	WAIT
};

int err  = 0;
int test = 0;

int TestIsReal(char *st);
int SymbolRetriver(char ch);

int main()
{
	if(1 != TestIsReal("+3258"))
    {
        ++err;
        printf("err on line %d\n", __LINE__);
    }

    if(1 != TestIsReal("3258"))
    {
        ++err;
        printf("err on line %d\n", __LINE__);   
    }

    if(1 != TestIsReal("1.1"))
    {
        ++err;
        printf("err on line %d\n", __LINE__);   
    }

    if(1 != TestIsReal(".1"))
    {
        ++err;
        printf("err on line %d\n", __LINE__);   
    }

    if(1 == TestIsReal("a.1"))
    {
        ++err;
        printf("err on line %d\n", __LINE__);   
    }

    test += 5;
    
	if(err > 0)
	{
		printf("failed it!!!\n");
	}
	else
	{
		printf("you aced it you got heck of state mechine\n");
	}

	return (0);
}

int TestIsReal(char *st)
{
    fsm_t *fsm = NULL;
    int symbol = 0;
    int result = 0;
    const transition_t transition_info_table[NUM_STATES * NUM_SYMBOLS] =
    { 
    	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL}, 
    	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL}, 
        {INVALID_CHAR, NULL},

    	{FINISH_SUCCESFULLY, NULL}, {FINISH_SUCCESFULLY, NULL},
    	{FINISH_SUCCESFULLY, NULL}, {FINISH_SUCCESFULLY, NULL},
        {FINISH_SUCCESFULLY, NULL},

    	{INVALID_CHAR, NULL}, {GOT_DIGIT_AFTER_DOT, NULL}, 
    	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL},
        {INVALID_CHAR, NULL},

    	{INVALID_CHAR, NULL}, {GOT_DIGIT_BEFORE_DOT, NULL},
    	{GOT_DOT, NULL}, {INVALID_CHAR, NULL},
        {INVALID_CHAR, NULL},


       	{INVALID_CHAR, NULL}, {GOT_DIGIT_BEFORE_DOT, NULL}, 
       	{GOT_DOT, NULL}, {FINISH_SUCCESFULLY, NULL},
        {INVALID_CHAR, NULL},

        {INVALID_CHAR, NULL}, {GOT_DIGIT_AFTER_DOT, NULL}, 
        {INVALID_CHAR, NULL}, {FINISH_SUCCESFULLY, NULL},
        {INVALID_CHAR, NULL},

        {GOT_SIGN, NULL}, {GOT_DIGIT_BEFORE_DOT, NULL}, 
        {GOT_DOT, NULL}, {INVALID_CHAR, NULL},
        {INVALID_CHAR, NULL},

    };
    const unsigned int initail_state = WAIT;
    const unsigned int accept_states[] = {FINISH_SUCCESFULLY};

    fsm = FsmCreate(transition_info_table,
                    accept_states, ACCEPT_STATE_NUM, NUM_STATES,
                    NUM_SYMBOLS, initail_state, NULL);
    while (symbol != FINISH_SYMBOL &&
           symbol != INVALID_SYMBOL)
    {
        symbol = SymbolRetriver(*st);
        FsmNext(fsm, symbol);
        ++st;
    }
    result = FsmIsAccepted(fsm);
    FsmDestroy(fsm);

    return result;
}

int SymbolRetriver(char ch)
{
	if (('0' <= ch) && ('9' >= ch))
	{
		return (DIGIT_SYMBOL);
	}
	else if (('+' == ch) || ('-' == ch))
	{
		return SIGN_SYMBOL;
	}
	else if ('\0' == ch)
	{
		return FINISH_SYMBOL;
	}
    else if ('.' == ch)
    {
        return DOT_SYMBOL;
    }
	else
	{
		return INVALID_SYMBOL;
	}
}