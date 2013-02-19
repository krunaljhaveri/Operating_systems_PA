/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;
	struct mblock *p, *q;
	int flag=0;
	unsigned int tmpsize;
	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);
	
	/* explicitly freeing the list by killing */
		
		p = &(pptr->p_head) ;
		struct mblock *temp;
		if (p == (struct mblock *) NULL) 
		{
			return(SYSERR);
		}
		
		temp = p;
		p=p->mnext;
		
		while(p!=NULL)
		{			
			temp=p->mnext;
			if(freemem(p,(p->mlen))==SYSERR)
			{
				//kprintf("SYSERR -------------------------------------------------->\n");
				flag =1;
			}
			else if(freemem(p,(p->mlen))!=SYSERR)
			{ 
				freemem(p,(p->mlen));
			}
		
			p=temp;	
		}
			
	
	if(flag==1)
	return(SYSERR);
	
	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
