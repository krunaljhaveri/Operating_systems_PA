#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <stdio.h>
#include <proc.h>
WORD *getmemgb(unsigned nbytes)

{
		//kprintf(" entering getmemgb\n");
		STATWORD ps; 
		struct mblock *p, *q;
		disable(ps);
		p = &proctab[currpid].p_head ;
		q = p->mnext;
		WORD* proc_word = getmemfn(nbytes +8);
		if (nbytes ==0 || p == (struct mblock *) NULL) 
		{
		return(SYSERR); 
		}
		
		while( p!=NULL && q != NULL)
		{	
			p = q;
			
			q = q->mnext;
		}
		p->mnext = (struct mblock *)proc_word;
		p->mnext->mlen= nbytes+8;
		p->mnext->mnext=NULL;
		restore(ps);
		return (proc_word +2);
		

}
