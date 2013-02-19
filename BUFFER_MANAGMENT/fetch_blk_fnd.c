#include <kernel.h>
#include <pa4.h>

dsk_buffer_p fetch_blk_fnd()
{
		
	if ( policy == POLICY_FIFO)
	{
		kprintf("\n-----------entering blk fnd fifo---------");
		kprintf("\n the value of tmp is %u",tmp);
		return tmp;
	}
	else if ( policy == POLICY_LRU)
	{
		tmp = buf_head;
		while(tmp->next!=NULL)
		{
			kprintf("\n-----------entering blk fnd lru---------");
			kprintf("\n the value of tmp is %u",tmp);
					
				if(tmp->next->block_no==block_no && tmp->next->pdev == pdev )
				{	
					kprintf("\n entering find prev&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
					prev = tmp;
					kprintf("\n prev value in findprev is %u",prev);
					break;	
				}		
				if ( tmp == buf_head && buf_head->block_no == block_no && buf_head->pdev == pdev)
				{
					kprintf("\n JKSLLLLLLLLLLLLLLLLLLLLL FIRST JKNSIONIOOOOIO");
					prev = buf_head;
					kprintf("\n value of prev is %u \n", prev);
					break;
				}
				tmp = tmp->next;
		}
				
		if ( prev != NULL)
		{
			kprintf("\n entering reorder of prev************************************************88 \n");
			tmp = prev->next;
			kprintf("\n value of tmp assigning prev ->next is %u", tmp);
			if ( tmp->block_no == block_no && tmp->pdev == pdev)
			{	
				kprintf("\n entering if loop of finding  prev************************************************88 \n");
				prev->next = tmp-> next;
				tmp->next =buf_head;
				buf_head = tmp;
			}
		}
		kprintf("\n RETURNING BUF _HEAD\n");
		return buf_head;
		
	}
	return INVALID_BLOCK;
}