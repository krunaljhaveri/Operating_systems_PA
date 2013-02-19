#include <kernel.h>
#include <pa4.h>
#include <proc.h>

extern int dskread(struct devsw *pdev, char *buffer, int block_no, int count);
extern int dskwrite(struct devsw *pdev, char *buffer, int block_no, int count);

extern int memncpy(void *dest, void *src, int num);
/*
 * Part A 3/4. buf_fetch()
 * buf_fetch() does:
 *     if there is a buffer that already store the block data of block_no,
 *         then return the buffer.
 *     otherwise
 *         read the block of block_no, store it on a buffer, and return the buffer.
 * 
 * parameters:
 *     pdev:		device descriptor
 *     block_no: 	a block number to read
 *     policy: 		buffer replacement policy
 */
dsk_buffer_p buf_fetch(struct devsw *pdev, int block_no, int policy) 
{
	
	
	dsk_buffer_p	tmp, prev, buf, nextbuf, victim;
	STATWORD ps;    
	//disable(ps);
	tmp = buf_head;
	prev = buf_head;
	int fnd=0;
	while(tmp!=NULL)
	{
		if(tmp->block_no == block_no && tmp->pdev == pdev)
		{
			fnd =1;
			break;
		}
		tmp = tmp->next;
	}
	if( fnd ==1)
	{
			
			//wait(tmp->sem_b);
			if( policy == POLICY_LRU)
			{
				//disable(ps);
				re_order(tmp);
				//restore(ps);
			}
			//restore(ps);
			return tmp;
	}
	else
	{
		int presize,found=0 ,count;
		char *mybuf;
			
			if((block_no + PA4_PREFETCH_SIZE +1)<=1024)
			{
				//kprintf("\n valid fetch size");
				count = PA4_PREFETCH_SIZE + 1;
				mybuf = getmem(128*count);
				
			}
			else
			{
				//kprintf("\n invalid fetch size");
				count = 1024 - block_no +1;
				mybuf = getmem( 128*count);	
			}
				
				
			
			for( presize =0;presize <count; presize++)
			{
					for(tmp = buf_head;tmp!=NULL;tmp = tmp->next)
				{
					
					if(tmp->block_no ==(block_no+presize) && tmp->pdev == pdev && tmp->d_bit == 1)
					{
							
							
							//wait(tmp->sem_b);
							//restore(ps);
							dskwrite(tmp->pdev,tmp->data,tmp->block_no,1);
							//disable(ps);
							
							tmp->d_bit =-1;
							//signal(tmp->sem_b);
						//			
							
							break;
					}
				}
			}
			
			//restore(ps);
			
			dskread(pdev,mybuf,block_no,count);
			
			//disable(ps);

				
				for(tmp = buf_head;tmp!=NULL;tmp = tmp->next)
				
				{
					if( tmp->block_no ==block_no && tmp->pdev == pdev )
					{
							

							//wait(tmp->sem_b);
								buf = tmp;
								break;
					}
				}
			
				
				
				

				
			for( presize =0;presize <count; presize++)
			{	
				for(tmp = buf_head;tmp!=NULL;tmp = tmp->next)
				
				{
					if( tmp->block_no ==(block_no+presize) && tmp->pdev == pdev )
					{
						found = 1;
						
						//wait(tmp->sem_b);
						break;
					
					}
				
				}
				
				if(found ==1 )
				{
					found=0;
					//if(policy == POLICY_LRU)
					//re_order(tmp);
				
				
				}
				if(found == 0 )
				{
					//disable(ps);
					victim = buf_victim();
					
					//wait(victim->sem_b);
					//restore(ps);
					if(victim->d_bit == 1)
					{
						//restore(ps);
						dskwrite(victim->pdev,victim->data,victim->block_no,1);
						//disable(ps);
					}
					memncpy(victim->data,(mybuf+(presize*victim->size)),victim->size);
					//signal(tmp->sem_b);
					
					
					
					victim-> pdev = pdev;
					victim->block_no = block_no + presize;
					victim->d_bit = -1;
					if(victim->block_no== block_no && victim->pdev == pdev)
						{
							buf =victim;		
						}
						
					//disable(ps);
					re_order(victim);
					
				}
				
			}	
		freemem(mybuf,128*count);
		//restore(ps);
		return buf;
	}
	
	return INVALID_BLOCK;
}