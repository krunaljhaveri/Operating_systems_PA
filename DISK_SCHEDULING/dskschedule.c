#include <kernel.h>
#include <pa4.h>

/*
 * Part B 1/1. dskschedule()
 * This function should be implemented to support each disk scheduling policy.
 * Parameters:
 *     ptr:	a descriptor of each device information
 *     option:	a disk scheduling policy to be used
 */
void dskschedule(disk_desc *ptr, int option) 
{
	//kprintf("\n dsk sched");
	if ( option == DISK_SCHED_FIFO)
	{
		return ;
	}
	
	else if ( option == DISK_SCHED_SSTF )
	{
		//kprintf("\n ____________________________SSTF---------------------------");
		request_desc_p tmp, rqhead, currptr, prev, currprev, tail;
		rqhead = ptr->request_head;
		int minimum,headnow, currdist;
		headnow=ptr->head_sector;
		minimum = headnow - rqhead->block_no;
		if(minimum <0)
			minimum = minimum * -1;
		//kprintf("\n minimum is %d",minimum);
		currptr = rqhead;
		currprev = NULL;
		
		
		
		for ( tmp = rqhead, prev = NULL;tmp != NULL;prev= tmp,tmp = tmp->next)
		{
			//kprintf("\n ____________________________enterinf for---------------------------");
			currdist = headnow - tmp->block_no;
			if(currdist <0)
				currdist = currdist * -1;
			//kprintf("\n currdist is %d",currdist);
			if ( currdist<minimum)
			{
				minimum = currdist;
				currptr = tmp;
				currprev = prev;
				
			}
			//kprintf(" tail elt is %d",prev->block_no);
			
		}
		// only block
		if( currprev == NULL && rqhead->next == NULL)
		{
			//kprintf(" if only elt \n");
			return;
		}
		//first block
		if( currprev == NULL && rqhead->next != NULL)
		{
			//kprintf(" if first elt \n");
			prev->next = currptr;
			
			ptr->request_head = rqhead = rqhead->next;
			
			currptr->next = NULL;
		}
		//tail block
		if( currptr == prev)
		{
			return;
		
		}
		
		// any block
		if(currprev!=NULL)
		{
			//kprintf(" gen elt \n");
			currprev->next = currptr->next;
			prev->next = currptr;
			currptr->next = NULL;
		} 
		
		
			
	}
	else if ( option == DISK_SCHED_CLOOK)
	{
		
		request_desc_p tmp, rqhead, minptr, prev, headptr, theblock, target, target_prev;
		rqhead = ptr->request_head;
		headptr = ptr;
		int minblk, headnow, currblk, nextblk, block_diff;
		headnow = ptr->head_sector;
		minblk = rqhead->block_no;
		
		int min = MAXINT, found = 0;
		
		//only one block
		if(rqhead->next == NULL)	return;
		
		for(theblock = rqhead,prev = NULL ;theblock != NULL;prev = theblock,theblock = theblock->next)
		{
			block_diff = theblock->block_no - headnow;
			if(block_diff < 0)
				continue;
			if(min > block_diff)
			{
				min = block_diff;
				target = theblock;
				target_prev = prev;	
				found = 1;					
			}
		}
		min = MAXINT;
		if(found == 0)
		{
			for(theblock = rqhead,prev = NULL ;theblock != NULL;prev = theblock,theblock = theblock->next){
				if(min > theblock->block_no)
				{
					min = theblock->block_no;
					target = theblock;
					target_prev = prev;
				}
			}
		}
		
		// only elt
		if(target_prev == NULL && rqhead->next == NULL)
		{
				return;
		}
		// first elt
		if(target_prev == NULL && rqhead->next != NULL)
		{
			prev->next = target;
			ptr->request_head = rqhead = rqhead->next;
			target->next = NULL;
		}	
		// tail elt	
		if( target == prev)
		{
			return;
		}
		if(target_prev != NULL)
		{
			target_prev->next = target->next;
			prev->next = target;
			target->next = NULL;
		
		}
		
		/*for(theblock = headnow ; theblock->next != NULL; theblock = theblock->next);
		theblock->next = target;*/
	} 

} 