
#include <kernel.h>
#include <pa4.h>


void re_order(dsk_buffer_p buf)
{
		dsk_buffer_p tmp, prev;
		tmp = buf_head;
		prev = NULL;
		
		if(buf == buf_head)
		{
			//kprintf("\n spl case");
			return;
		}
		
		while(tmp!=NULL)
		{
			//kprintf("\n reorder >> while");
			if(tmp == buf)
			{
				prev->next = tmp->next;
				tmp->next = buf_head;
				buf_head = tmp;
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}


}

dsk_buffer_p buf_victim(void)
{
	dsk_buffer_p	tmp, victim;
	tmp = buf_head;
	int i=0;
	if( buf_count< PA4_BUFFER_SIZE)
	{
		while( i<buf_count)
		{
			tmp = tmp->next;
			i++;
		}
		buf_count++;
		return tmp;
	}
	else if ( buf_count == PA4_BUFFER_SIZE)
	{
		while( tmp->next != NULL )
		{
			tmp=tmp->next;
		}
		victim = tmp;
		return victim;
	}			
	
}

