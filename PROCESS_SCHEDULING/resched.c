
/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *	Proctab[currpid].pstate gives correct NEXT state for
 *	current process if other than PRREADY.
 *------------------------------------------------------------------------
 */

int resched()
{
register struct	pentry	*optr;	/* pointer to old process entry */
register struct	pentry	*nptr;	/* pointer to new process entry */

	int next_ppid=0;
	// setting to aging scheduling
	if(scheduleclass == AGINGSCHED)
	{	
	
		int i;
		/* setting the priority for aging processes in ready queue */
		for (i = 1; i < NPROC; i++)
		{
			if(proctab[i].pstate == PRREADY)
			{
			proctab[i].cur_pprio = min(99, ceil((proctab[i].cur_pprio) * 1.2));
			}
		}
		// runtime priority is set to base priority for currpid
		proctab[currpid].cur_pprio = proctab[currpid].pprio;
		// the highest priority process is set as next_ppid
		
		next_ppid = highPrio();			
		// if current and next process are the same preempt the clock
		if( next_ppid == currpid)
		{
			#ifdef RTCLOCK
			preempt = QUANTUM;
			#endif
			return(OK);
		}
		// defining old and new process entry
		optr = &proctab[currpid];
		nptr= &proctab[next_ppid];
	
		/* force context switch */

		if (optr->pstate == PRCURR)
		{
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,optr->pprio);
        }

		
		
		/* remove the highest priority at end of queue */
	
		nptr = &proctab[ (currpid = dequeue(next_ppid)) ];

		nptr->pstate = PRCURR;
		currpid = next_ppid;

		#ifdef RTCLOCK
		preempt = QUANTUM;
		#endif
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		/* The OLD process returns here when resumed. */
		return(OK);
	}
	/* setting to linux like scheduling*/
	else if(scheduleclass == LINUXSCHED)
	{
		//kprintf("I\n");
		int i = 0;
		// setting the counter , quantum and epoch
		proctab[currpid].pcounter =  preempt;
		nepoch = nepoch - (proctab[currpid].pquantum-preempt);
		/*check if process is in new epoch */
		if(isnewepoch()==1)
		{
			nepoch=0;
			// set counter, quantum and epoch for the processes in the current epoch
			setcounter();
			
		} 
		
		//pick the next highest priority process 
		next_ppid = nextHighPr();
		//int esum = epoch();
		/* inserting the current process in the queue */
		if (optr->pstate == PRCURR)
		{
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,optr->pprio);
        }
		// current and next process are the same set the preempt to counter
		if(next_ppid==currpid)
		{
			preempt = proctab[next_ppid].pcounter;
			return(OK);
		}
		else
		
		{
			//defining old and new process entry pointers
			optr = &proctab[currpid];
			nptr = &proctab[next_ppid];
			if (optr->pstate == PRCURR)
			optr->pstate = PRREADY;
			currpid = next_ppid;
			nptr->pstate = PRCURR;
			// REMOVE THE HIGHEST PRIORITY FROM END OF QUEUE
			nptr = &proctab[ (currpid = dequeue(next_ppid)) ];

			//kprintf("\nnptr->counter = %d\tpreempt = %d\n",nptr->counter,preempt);
			#ifdef  RTCLOCK
			preempt = nptr->pcounter;              /* reset preemption counter     */
			#endif
			ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return(OK);
		}
	}
	/* SETTING TO DEFAULT XINU SCHEDULER */
	else if ( scheduleclass == DEFAULT)
	{
		/* no switch needed if current process priority higher than next*/

		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
		(lastkey(rdytail)<optr->pprio)) {
		return(OK);
		}
		/* force context switch */

		if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
		}

		/* remove highest priority process at end of ready list */

		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		nptr->pstate = PRCURR;	/* mark it currently running	*/
		#ifdef	RTCLOCK
		preempt = QUANTUM;	/* reset preemption counter	*/
		#endif
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		/* The OLD process returns here when resumed. */
		return OK;
	}
	
	return OK;
}
