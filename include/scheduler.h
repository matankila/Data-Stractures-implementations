/********************************************/
/*											*/
/*					OL 61					*/
/*					Scheduler				*/
/*											*/
/********************************************/

#ifndef OL61_SCHEDULER
#define OL61_SCHEDULER

#include "uid.h"

typedef struct sch_t sch_t;

typedef int (* sch_task_func)(void *args);

/*
*	return value - pointer to the scheduler management struct
*	arguments - none
*	this function creates a scheduler management struct
*/
sch_t *SchCreate(void);

/*
*	return value - none
*	arguments - scheduler management struct
*	this function receives a schedular management struct
*	and frees its allocated memory
*/
void SchDestroy(sch_t *sch);

/*
*	return value - unique id struct
*	arguments - scheduler management struct, task function pointer, 
*	pointer to task function arguments, timing value
*	this function adds a new task into the schedule, with the given
*	arguments, where timing determines the place in the schedule
*/
nuid_t SchAdd(sch_t *sch,
              sch_task_func task,
              void *args,
   struct timeval timing);

/*
*	return value - unique id struct
*	arguments - scheduler management struct, unique id struct
*	this function finds and removes a task with the received unique id 
*	from the schedule, returning the id of the removed task
*/
nuid_t SchRemove(sch_t *sch, nuid_t id);

/*
*	return value - return status
*	arguments - scheduler management struct
*	this function starts the execution phase of the schedule, allowing
*	it to start executing scheduled tasks
*/
int SchRun(sch_t *sch);

/*
*	return value - return status
*	arguments - scheduler management struct
*	this function stops the execution phase of the schedule
*/
int SchStop(sch_t *sch);

/*
*	return value - number of tasks in the schedule
*	arguments - scheduler management struct
*	this function returns the number of tasks in the schedule
*/
size_t SchSize(const sch_t *sch);

/*
*	return value - boolean value
*	arguments - scheduler management struct
*	this function returns 1 if the provided schedule is empty
*	and 0 otherwise
*/
int SchIsEmpty(const sch_t *sch);

/*
*	return value - void
*	arguments - scheduler management struct
*	this function clears all remaining tasks from the schedule
*/
void SchClear(sch_t *sch);

#endif /* OL61_SCHEDULER */