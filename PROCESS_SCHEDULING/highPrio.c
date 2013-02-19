#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>
/* highest priority process */ 
int  highPrio()
{	
	int i,highprio=0,next_ppid=0;
	for(i=1;i<NPROC;i++)
	{
		if(proctab[i].pstate == PRREADY || proctab[i].pstate == PRCURR)
		{
			if(proctab[i].cur_pprio > highprio)
				{
					highprio = proctab[i].cur_pprio;
					next_ppid = i;
				}
		}
	}
	return next_ppid;
}
