/********************************************/
/*											*/
/*					OL 61					*/
/*					Scheduler				*/
/*											*/
/********************************************/

#ifndef OL61_SCHEDULER_TASK
#define OL61_SCHEDULER_TASK

#include "uid.h"

typedef int (* task_func)(void *args);

typedef struct task_t task_t;

/*
*	return value - pointer to task
*	arguments - task function pointer, pointer to task function arguments, timing value
*	this function creates a new task, with the provided arguments
*/
task_t *TaskCreate(task_func task, void *args, struct timeval timing);

/*
*	return value - unique id struct
*	arguments - pointer to a specific task
*	this function destroys the given task, returning its id
*/
nuid_t TaskDestroy(task_t *task);

/*
*	return value - unique id struct
*	arguments - const pointer to task
*	this function receives a pointer to a task, and returns
*	a pointer to the unique id of that task
*/
nuid_t TaskGetUID(const task_t *task);

/*
*	return value - timeval struct
*	arguments - const pointer to task
*	this function receives a pointer to a task, and returns
*	the time of its next iteration
*/
int TaskGetNextRunTime(const task_t *task);

/*
*	return value - void
*	arguments - pointer to task
*	this function receives a pointer to a task to be re added
*	to the schedule, and updates its next run time
*/
void TaskUpdateNextRunTime(task_t *task);

/*
*	return value - return value of the task
*	arguments - pointer to task
*	this function receives a task pointer, and executes it
*	returning the return value of that task
*/
int TaskRun(task_t *task);


#endif /* OL61_SCHEDULER_TASK */