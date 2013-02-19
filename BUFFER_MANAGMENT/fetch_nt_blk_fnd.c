#include <kernel.h>
#include <pa4.h>

dsk_buffer_p fetch_nt_blk_fnd()
{
		char buffer[128];
		dskread( pdev,buffer,block_no,1);
		kprintf(" buffer in buf_fetch is %s\n",buffer);
		victim = buf_victim();
				if ( victim->d_bit == 1)
			{
				dskwrite(victim->pdev,victim->data,victim->block_no, 1);
			}
			kprintf("\n data in victim  b4 memncpy is %s\n",victim->data);
			memncpy(victim->data,buffer,128);
			kprintf("\n data in victim  after memncpyis %s\n",victim->data);
			victim-> pdev = pdev;
			victim->block_no = block_no;
			victim->d_bit = -1;
			tmp = buf_head;
			while(tmp!= victim)
				{
					prev = tmp;
					tmp = tmp->next;
				}
				kprintf(" prnt<<<<<<<<<<<<<<<<<<<<,reorder\n");
				prev->next = victim->next;
				victim->next = buf_head;
				buf_head = victim;
				kprintf("buffer data in buf_fet b4 return is %s\n",buf_head->data);
		
			return buf_head;
}	