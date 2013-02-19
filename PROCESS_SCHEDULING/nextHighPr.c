#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<stdio.h>
/* next high priority process with best goodness */

int nextHighPr()
{
		
	int i,max = 0,ppid=0,goodness;
	for(i = 0;i < NPROC;i++)
	{
		if(proctab[i].pstate == PRCURR || proctab[i].pstate == PRREADY)
		{	
			// comopute goodness
			goodness = compgoodness(i);
			// process id with best goodness
			if(goodness > max)
			{
				max = goodness;
				ppid = i;
				
			}
		}
	}
	return ppid;

}
