#include <kernel.h>
#include <pa4.h>
extern int disk0_preempt, disk1_preempt;
// CALL the following function from main...
//THIS will WORK only if the following statement is included in dskinterrupt just before the line having
// freemem .. the code will look as follows ....
/*
        ready(target -> process_id, RESCHNO);
        kprintf("%d,",target->block_no);
        //rvprintf("%d,",target->block_no);
        freemem(target, sizeof(request_desc));
*/
int testdiskschedule(int policy)
{
        request_desc_p tmp,head;
        head=NULL;
        int disk=DISK0;
       
        struct devsw* pdev;
        pdev = &devtab[DISK0];
        int i,processA;
        //int access[]={26,49,37,99,81,77,16,31,12};
        //int access[]={63,100,10,11,12,98,83,69,53,19,15,28,31};
        int access[] = {90,55,58,39,18,160,150,38,184};
        char *strt = "SENTENCE ";
        int j;
        for(j=0;j<9;j++)
        {
                resume(processA = create(process_new, 2000, 10, "Process A", 1, (char)"A"));
        }
               
        int some=49;
        for(i=0;i<9;i++)
        {
                tmp = getmem(sizeof(request_desc));
               
                tmp -> type = WRITE_OPERATION;
                tmp -> block_no = access[i];
                tmp -> process_id = some--;
                tmp -> buffer = strt;
                tmp -> count = 1;
                tmp -> next = head;
                head = tmp;
        }
        tmp =head;
        while(tmp!=NULL)
        {
                kprintf("%d ->",tmp->block_no);
                tmp=tmp->next;
        }
        kprintf("\n");
        kprintf("Attaching queue \n");
        if(policy==DISK_SCHED_FIFO)
        {
                kprintf("Expected Output : 90,55,58,39,18,160,150,38,184\n");
                kprintf("Actual Output   : ");
        }
        if(policy==DISK_SCHED_SSTF)
        {
                kprintf("Expected Output : 90,58,55,39,38,18,150,160,184\n");
                kprintf("Actual Output   : ");
        }
        if(policy==DISK_SCHED_CLOOK)
        {
                kprintf("Expected Output : 90,150,160,184,18,38,39,55,58\n");
                kprintf("Actual Output   : ");
        }
        STATWORD ps;
        disable(ps);
       
        disk_desc* ptr;
        ptr = pdev->dvioblk;
        request_desc_p target, prev;
        int ticks;
        double seek_time, rotate_time, transfer_time;
        for(target = ptr -> request_head;target -> next;target = target -> next);
                calculate_time(ptr, target -> block_no, &seek_time, &rotate_time);
                calculate_transfer_time(ptr, target -> block_no, target -> count, &transfer_time);
                ticks = ms_to_ticks(seek_time + rotate_time + transfer_time);
                target -> ticks = ticks;
                disk0_preempt = ticks;
                head->ticks=ticks;
        ptr->head_sector=100;
        ptr->request_head= head;
        //ptr->no_of_writes= 8;
        //rvprintf("AAA");
        restore(ps);
       
        open(disk, 0, 0);
        //dskwrite(pdev,strt,184,1);
        //kprintf("HHH");
        dskresched();
}
int process_new(char c)
{
        while(1)
        {
        }
}