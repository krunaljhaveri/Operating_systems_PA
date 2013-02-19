#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<math.h>
/* SETTING COUNTER AND QUANTUM VALUES */
void setcounter()
{
        int i;
	//kprintf(" cur is %d ", cur);
        //int counter;
	// loop through 1 to NPROC  processes
  	for(i=1;i<NPROC;i++)
	{		proctab[i].pprio = proctab[i].newprio;
			// process roll over
			if (proctab[i].pcounter >= threshold)
					proctab[i].pquantum = floor(proctab[i].pcounter/2) + proctab[i].pprio;
					else
					proctab[i].pquantum =  proctab[i].pprio;	// quantum = priority
					proctab[i].pcounter = proctab[i].pquantum;	// set counter
					nepoch = nepoch + proctab[i].pquantum;		
	}
}
