#include <stdio.h>
#include <kernel.h>
#include <pa4.h>

extern int currpid, disk0_preempt, disk1_preempt;

extern void print_double(double data);
extern void calculate_time(disk_desc *pdev, int block_no, double *seek_time, double *rotate_time);
extern void calculate_transfer_time(disk_desc *pdev, int block_no, int count, double *transfer_time);
extern int memncpy(void *desc, void *src, int num);
extern int ms_to_ticks(double ms);

/*
 * dskread() is responsible for serving each read request.
 * It does:
 *     check simple parameter integrity,
 *     calculate seek time, rotate time, and transfer time
 *     insert the current request into each queue
 *     schedule the interrupt related to this request
 *     make the current process into block state.
 * This function supports multiple continuous blocks.
 * Please notice that when calling this function, block_no + count does not have to
 * exceed the last block number. This function does not guarantee that block_no + count
 * does not exceed.
 * parameter:
 *     pdev:		device descriptor
 *     buffer:		a buffer to store read blocks
 *     block_no:	the start block number to be read
 *     count:		the number of continuous blocks to be read
 */
int dskread(struct devsw *pdev, char *buffer, int block_no, int count) {
	disk_desc *ptr;
	double seek_time = 0.0;
	double rotate_time = 0.0;
	double read_time = 0.0;
	int code, ticks;
	request_desc_p request;
	STATWORD ps;
	
	disable(ps);
	if(!pdev || !buffer || count <= 0) {
		restore(ps);
		return SYSERR;
	}
	
	ptr = (disk_desc *)pdev -> dvioblk;	
	if(block_no < 0 || block_no + count > ptr -> logical_blocks) {
		restore(ps);
		return SYSERR;
	}

	request = (request_desc_p)getmem(sizeof(request_desc));
	if(request == (request_desc_p)SYSERR) {
		restore(ps);
		return (int)request;
	}

	if(!ptr -> request_head) {
		calculate_time(ptr, block_no, &seek_time, &rotate_time);
		calculate_transfer_time(ptr, block_no, count, &read_time);
		ticks = ms_to_ticks(seek_time + rotate_time + read_time);
		if(pdev == &devtab[DISK0])
			disk0_preempt = ticks;
		else if(pdev == &devtab[DISK1])
			disk1_preempt = ticks;
		request -> ticks = ticks;
	}
	//kprintf("\n------------dsk read\n");
	request -> type = READ_OPERATION;
	request -> block_no = block_no;
	request -> process_id = currpid;
	request -> buffer = buffer;
	request -> count = count;
	request -> next = ptr -> request_head;
	ptr -> request_head = request;
	//kprintf("\n req buf is %s\n", request->buffer);
	restore(ps);
	if(dskresched() == SYSERR) {
		return SYSERR;
	}
	
	return OK;
}
