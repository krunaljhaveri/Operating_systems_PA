#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<stdio.h>
// CHECK IF IN NEW EPOCH
int isnewepoch()
{
	int i;
	
	// sets 1 for a new epoch if goodness is zero
	for(i = 1;i < NPROC ; i++)
	{
		if(proctab[i].pstate == PRCURR || proctab[i].pstate == PRREADY){
			
			if(compgoodness(i) > 0)
				return 0;	// OLD OR CURRENT EPOCH
		}
	}
	return 1; 	//NEW EPOCH
}	
