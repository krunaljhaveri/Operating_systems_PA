/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	//int scheduleclass;
	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if(scheduleclass==1)
	{
	pptr->pprio=newprio;
	pptr->cur_pprio=newprio;
	}
	else if (scheduleclass ==2)
	{
		pptr->newprio = newprio;
	}
	restore(ps);
	return(newprio);
}
