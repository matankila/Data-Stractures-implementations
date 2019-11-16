/************************************
*									*
*									*
*			Pre-Calc 			    *
*		 STATE MACHINE		        *
*									*
************************************/

#ifndef OL61_PRE_CALC
#define OL61_PRE_CALC

#include <stddef.h> /* for size_t */

typedef int (*func_t)(void *args);

typedef struct transition_info_t
{
	int next_state;
	func_t action_func;
}transition_t;

typedef struct fsm_t fsm_t;

/*
* return value - pointer to administrative struct.
* arguments - pointer to user's transitions table array, accept states array.
* hands over a pointer to pointer to administrative struct.
*/
fsm_t *FsmCreate(const transition_t *transition_t_table, 
	             const unsigned int *accept_states,
	             size_t number_of_accept_states, size_t number_of_states,
	             size_t number_of_symbols, unsigned int intial_state,
	             void *params);

/*
* arguments - pointer to pointer to administrative struct.
* Free the memory of the administrative struct.
*/
void FsmDestroy(fsm_t *fsm);

/*
* return value - Returns the current accepted status.
* arguments - pointer to user's transitions table array.
* check the current state and return accepted or not accepted.
*/
int FsmIsAccepted(fsm_t *fsm);

/*
* arguments - pointer to pointer to administrative struct, current symbol.
* Advance the current state to the next one.
*/
void FsmNext(fsm_t *fsm, int symbol);

#endif /* OL61_PRE_CALC */