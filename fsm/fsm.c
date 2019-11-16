#include <assert.h>   /* assert       */
#include <string.h>   /* strlen       */
#include <stdlib.h>   /* memory alloc */
#include "fsm.h"      /* my header    */

struct fsm_t
{
	const transition_t *transition_t_table;
	const unsigned int *accept_states;
	size_t number_of_accept_states;
	size_t number_of_states;
	size_t number_of_symbols;
	unsigned int current_state;
	void *param;
};

/*
* return value - pointer to administrative struct.
* arguments - pointer to user's transitions table array, accept states array.
* hands over a pointer to pointer to administrative struct.
*/
fsm_t *FsmCreate(const transition_t *transition_t_table, 
	             const unsigned int *accept_states,
	             size_t number_of_accept_states, size_t number_of_states,
	             size_t number_of_symbols, unsigned int intial_state, 
	             void *param)
{
	fsm_t *fsm = NULL;

	assert(transition_t_table);
	assert(accept_states);
	assert(number_of_accept_states);
	assert(number_of_states);
	assert(number_of_symbols);

	fsm = malloc(sizeof(fsm_t));

	if(!fsm)
	{
		return NULL;
	}

	fsm->transition_t_table = transition_t_table;
	fsm->accept_states = accept_states;
	fsm->number_of_accept_states = number_of_accept_states;
	fsm->number_of_states = number_of_states;
	fsm->number_of_symbols = number_of_symbols;
	fsm->current_state = intial_state;
	fsm->param = param;

	return fsm;
}

/*
* arguments - pointer to pointer to administrative struct.
* Free the memory of the administrative struct.
*/
void FsmDestroy(fsm_t *fsm)
{
	assert(fsm);

	free(fsm);
	fsm = NULL;
}

/*
* return value - Returns the current accepted status.
* arguments - pointer to user's transitions table array.
* check the current state and return accepted or not accepted.
*/
int FsmIsAccepted(fsm_t *fsm)
{
	size_t i = 0;
	int return_value = 0;

	assert(fsm);

	for (i = 0; i < fsm->number_of_accept_states; ++i)
	{
		return_value += (fsm->current_state == fsm->accept_states[i]);
	}

	return return_value;
}

/*
* arguments - pointer to pointer to administrative struct, current symbol.
* Advance the current state to the next one.
*/
void FsmNext(fsm_t *fsm, int symbol)
{
    int current_state = 0;
    int symbol_num = 0;
    func_t func = NULL;
    void *params = NULL;

	assert(fsm);

    current_state = fsm->current_state;
    symbol_num = fsm->number_of_symbols;
    fsm->current_state = (fsm->transition_t_table + 
                          symbol + (symbol_num * current_state))->next_state;
	func = (fsm->transition_t_table + 
           (symbol * symbol_num) + (current_state))->action_func;

    if (func)
    {
    	params = fsm->param;
    	func(params);
    }
}