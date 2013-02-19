#include <kernel.h>
#include <sem.h>
/* 
 * Part A 1/4. buf_init()
 * buf_init() is responsible for initializing buffer cache management.
 */
 
 
void buf_init(void) 
{


	buf_head =  ( struct buf *) getmem (sizeof(struct buf));
	buf_head ->data = getmem (128);
	buf_head->size = 128;
	buf_head->d_bit = -1;
	buf_head->sem_b = screate(1);
	buf_count = 0;
	int i,j;
	dsk_buffer_p b_head = buf_head;
	dsk_buffer_p tmp, prev;
	//prev = buf_head;
	
	for ( i=1;i<PA4_BUFFER_SIZE;i++)
	{
		tmp = ( struct buf *) getmem (sizeof(struct buf));
		b_head->next = tmp;
		
		tmp->d_bit =-1;
		tmp->next = NULL;
		//tmp->prev = b_head;
		tmp->pdev = NULL;
		tmp->data = getmem (128);
		tmp->block_no =-1;
		tmp->size = 128;
		tmp->sem_b = screate(1);
		buf_count++;
		b_head = tmp;

		
		
	}
	
	//buf_print();
	
	
	
	
	

}