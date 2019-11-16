#ifndef OL61_UID
#define OL61_UID

#include <sys/time.h>  /* timeval */
#include <sys/types.h> /* pid_t   */
#include <stddef.h>    /* size_t  */

/*User, Never change these fields, may cause distruction of the uniqness*/
typedef struct
{	
	pid_t pid;          
	struct timeval time;
	size_t counter;    
}nuid_t;

extern const nuid_t g_bad_uid;

/*generate UID*/
nuid_t UidCreate();

/*check if two uids aer the same*/
int UidIsSame(nuid_t uid1, nuid_t uid2);

/*a function that check if the uid is valid or not*/
int UidIsValid(nuid_t checked_uid);

#endif /*OL16_UID*/