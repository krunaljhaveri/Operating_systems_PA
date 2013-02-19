#include <kernel.h>
#include <pa4.h>

/*
 * Part A 4/4. buf_put()
 * buf_put() processes each write request.
 * If policy is POLICY_WRITE_THROUGH,
 *     then find a buffer that already stores the block_no block, stores new data to it
 *     and finally store the data into disk.
 * Otherwise (POLICY_DELAYED_WRITE),
 *     then you have to handle requests based on the policy.
 */
int buf_put(struct devsw *pdev, int block_no, char *buffer, int policy) 
{
	// write thru
	dsk_buffer_p	tmp, prev, buf, nextbuf,lastbuf;
		int block_missing =1;
	if( policy == POLICY_WRITE_THROUGH)
	{
		
		
			
			for(tmp = buf_head; tmp!= NULL;tmp = tmp->next)
			{
				
				if(tmp->block_no == block_no && tmp->pdev == pdev)
				{
					
					block_missing =1;
					memncpy(tmp->data,buffer,tmp->size);
					break;
					//dskwrite(pdev,tmp->data,tmp->block_no,1);
				}
				
			}
			if(block_missing ==1 && policy == POLICY_LRU)
			{
				re_order(tmp);
			}
			return dskwrite(pdev, buffer, block_no,1);
	}
		
	 if ( policy == POLICY_DELAYED_WRITE )
	{
		
		
			for(tmp = buf_head; tmp!= NULL;tmp = tmp->next)
			{
				
				if(tmp->block_no == block_no && tmp->pdev == pdev)
				{
					block_missing =0;
					memncpy(tmp->data,buffer,tmp->size);
					tmp->d_bit =1;
					break;
				}
				
			}
			if(block_missing ==1 && policy == POLICY_LRU)
			{
				re_order(tmp);
			}
			
			
			//return 0;
	}
	// if block is missing
	if ( block_missing == 1)
	{
		
			
				lastbuf = buf_victim();
			
				
				
				if( lastbuf->d_bit == 1)
				{
					
					dskwrite(lastbuf->pdev,lastbuf->data,lastbuf->block_no ,1);
				}
				re_order(lastbuf);
				memncpy(buf_head->data,buffer,buf_head->size);
				buf_head->pdev = pdev;
				buf_head->block_no = block_no;
				buf_head->d_bit=1;
	}
0		
		//return 0;
	

}