#include <kernel.h>
#include <pa4.h>


/*
 * Part A 2/4. buf_close()
 * buf_close() deallocates all the resources that buffer manager has used.
 */
void buf_close(void) 
{

	dsk_buffer_p tmp;

	for( tmp = buf_head; tmp != NULL;tmp= tmp->next)
	{
		freemem( tmp, sizeof( struct buf));
		sdelete(tmp->sem_b);
	}

}
