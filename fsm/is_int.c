#include <stdio.h> /* IO fucntion */ 
#include "fsm.h"   /* my header */

#define ACCEPT_STATE_NUM 1
#define NUM_SYMBOLS 4
#define NUM_STATES 5

enum symbols
{
	SIGN_SYMBOL,
	DIGIT_SYMBOL,
	FINISH_SYMBOL,
	INVALID_SYMBOL
};
enum states
{
	INVALID_CHAR,
	FINISH_SUCCESFULLY,
	GOT_DIGIT,
	GOT_SIGN,
	WAIT
};

int err  = 0;
int test = 0;

void TestIsInt();
int SymbolRetriver(char ch);

int main()
{
	TestIsInt();

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

void TestIsInt()
{
    fsm_t *fsm = NULL;
    int symbol = 0;
    int i = 0;
    char *arr[5] = {"+3258", "+325a8", "231A", "-23A1", "+2X31"};
    const transition_t transition_info_table[NUM_STATES * NUM_SYMBOLS] =
    { 
    	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL}, 
    	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL},
    	{FINISH_SUCCESFULLY, NULL}, {FINISH_SUCCESFULLY, NULL},
    	{FINISH_SUCCESFULLY, NULL}, {FINISH_SUCCESFULLY, NULL},
    	{INVALID_CHAR, NULL}, {GOT_DIGIT, NULL}, 
    	{FINISH_SUCCESFULLY, NULL}, {INVALID_CHAR, NULL},
    	{INVALID_CHAR, NULL}, {GOT_DIGIT, NULL},
    	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL},
       	{GOT_SIGN, NULL}, {INVALID_CHAR, NULL}, 
       	{INVALID_CHAR, NULL}, {INVALID_CHAR, NULL} 
    };
    const unsigned int initail_state = WAIT;
    const unsigned int accept_states[] = {FINISH_SUCCESFULLY};

    for(i = 0; i < 5; i++)
    {
    	fsm = FsmCreate(transition_info_table,
                        accept_states, ACCEPT_STATE_NUM, NUM_STATES,
                        NUM_SYMBOLS, initail_state, NULL);
    	
    	if(!fsm)
    	{
    		printf("\33[1;31mFsmCreate failed in line %d\33[0;0m\n", __LINE__);
        	++err;
    		break;
    	}

    	while (symbol != FINISH_SYMBOL &&
               symbol != INVALID_SYMBOL)
    	{
        	symbol = SymbolRetriver(*arr[i]);
        	FsmNext(fsm, symbol);
        	++arr[i];
    	}

    	if(0 == i)
    	{
    		if (!FsmIsAccepted(fsm))
     		{
        		printf("\33[1;31mIsInt failed in line %d\33[0;0m\n", __LINE__);
        		++err;
    		}
    	}
    	else
    	{
    		if (FsmIsAccepted(fsm))
     		{
        		printf("\33[1;31mIsInt failed in line %d\33[0;0m\n", __LINE__);
        		++err;
    		}
    	}

    	FsmDestroy(fsm);
    }
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
	else
	{
		return INVALID_SYMBOL;
	}
}