#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<stdio.h>

// compute goodness for the given process id
int compgoodness(int ppid)
{
	int goodness=-1;
	//while(proctab[cur].pstate!=PRFREE)
		if(proctab[ppid].pcounter<=0)
		goodness =0;
		else
		{
			//GOODNESS = COUNTER + PRIORITY
		  goodness=proctab[ppid].pcounter+proctab[ppid].pprio;
   		}
	return goodness;
}	





