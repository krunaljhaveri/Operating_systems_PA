#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <stdio.h>
#include <proc.h>
SYSCALL freememgb(struct mblock *block, unsigned size)
{
		STATWORD ps; 
		struct mblock *p, *q, *temp, *release, *tmpnext;
		p = &proctab[currpid].p_head ;
		release=block;
		
		if (size ==0 || p == (struct mblock *) NULL) 
		{
			return(SYSERR);
		}	
		disable(ps);
		while((WORD*)p!=(int*)(release-1) && p!=NULL)
			{		
					temp=p;			
					p=p->mnext;		
			}
				
			
			if(p != NULL && size!=0)
				{
					
					
					if( (size+8)== p->mlen && release == (p+1))
					{
						
						tmpnext = p->mnext;				
						unsigned int a = freememfn(p,p->mlen);
						if(a == (unsigned) SYSERR)
						{ 
							return(SYSERR);
						}
						temp->mnext = tmpnext;
						
					}
					else
					{
					
						return SYSERR;
					}
					
				}
			 
			else if(p==NULL)
				{
					
					return(SYSERR);
				}
	restore(ps);						
	return(OK);
}