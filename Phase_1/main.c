// main.c, 159
// the kernel is simulated, not yet real
//
// Team Name: ??????? (Members: ?????? and ??????)

#include "spede.h"      // spede stuff
#include "main.h"       // main stuff
#include "isr.h"        // ISR's
#include "toolfunc.h"   // handy functions for Kernel
#include "proc.h"       // processes such as IdleProc()
#include "typedef.h"    // data types
#include "entry.h"
#include "TF.h"

// kernel data stuff:
int running_pid;            // currently-running PID, if -1, none running
q_t ready_q, free_q;        // processes ready to run and ID's un-used
pcb_t pcb[MAX_PROC_NUM];    // process table
char proc_stack[MAX_PROC_NUM][PROC_STACK_SIZE]; // runtime stacks of processes

struct i386_gate *IDT_ptr;

void SetEntry(int entry_num, func_ptr_t func_ptr){
	struct i386_gate *gateptr = &IDT_ptr[entry_num];
	fill_gate(gateptr, (int)func_ptr, get_cs(), ACC_INTR_GATE, 0);
}

int main() {
   	//int pid;

   	//call InitKernelData()  to set kernel data

	//Sets kernel data.
	InitKernelData();
	//Sets kernel control.
	InitKernelControl();

	/*call DeQ() to dequeue free_q to get pid
 	
   	call StartProcISR(pid) to create IdleProc

   	infinite loop to alternate 2 things below:
      		call LoadRun() to load/run the chosen process
      		call KernelMain() to run kernel periodically to control things

   	return 0;   // not reached, but compiler needs it for syntax*/
	//Dequeue a PID from the free_Q
	running_pid = DeQ(&free_q);
	//Create an idle process with this PID
	StartProcISR(running_pid);
	
	LoadRun(pcb[0].TF_ptr); // load/run IdleProc	
	/*while(1){
		LoadRun();
		KernelMain();
	}*/

	return 0;
}

void InitKernelData() {
   	int i;

   	//call MyBzero() to clear queues (which is to be coded in toolfunc.h/.c)
	
	// lopp number i from 0 to 19
   	for(i = 0; i < 19; i++){
      		//call EnQ() to enqueue i to free_q
      		EnQ(i, &free_q);
      		//call MyBzero() to clear pcb[i]
      		MyBzero((char *) &pcb[i], sizeof(pcb_t));
	}
   	//set running_pid to 0;  none initially, need to chose by Scheduler()
   	running_pid = 0;
}

void InitKernelControl(){ 
	IDT_ptr = get_idt_base();	// get where IDT is
	
	SetEntry(32, TimerEntry);	// prime IDT entry
	outportb(0x21, ~1);		// 0x21 is PIC mask, ~1 is mask
	//asm("sti");			// set/enable intr in CPU EFLAGS reg 	
}

void Scheduler() {  // to choose running PID
   /*simply return if running_pid is greater than 0 (0 or less/-1 continues)
   if running process ID is 0 (IdleProc), change its state to READY (from RUN)

   set running process ID = dequeue ready_q
   if it's -1 (didn't get one, ready_q was empty)
      set running process ID = 0 (fall back to IdleProc)

   whoever's now selected as running process, set its state to RUN*/

	if(running_pid>0){
		return;
	}else if(running_pid == 0){
		pcb[running_pid].state = READY;	
		
	}
	running_pid= DeQ(&ready_q);

	if(running_pid == -1){
		running_pid= 0;		
	}
	pcb[running_pid].state = RUN;
}

void KernelMain(TF_t *TF_ptr) {
   int new_pid;
   char key;

   /*call TimerISR() to service timer interrupt as if it just occurred

   if a key has been pressed on PC {
      read the key with cons_getchar()
      switch(key) {
         case 's'
            dequeue free_q for a new pid
            if the new pid (is -1) indicates no ID left
               show msg on target PC: "Panic: no more available process ID left!\n"
            else
               call StartProcISR(new pid) to create new proc
            break;

         case 'e'
            call EndProcISR() to handle this event
            break;

         case 'b'
            call breakpoint(); to go into GDB
            break;

         case 'x'
            just call exit(0) to quit MyOS.dli
     }
   }
   call Scheduler() to choose next running process if needed*/
	
	pcb[running_pid].TF_ptr = TF_ptr;
	
	switch(TF_ptr->intr_id){
		
		case TIMER_INTR:
			TimerISR();
			//D:
			outportb(0x20,0x60);
			break;
		default: 
			cons_printf("Panic: unknown intr ID (%d)!\n", TF_ptr->intr_id);
			breakpoint();	// fallback to GDB 
	}
	
	if(cons_kbhit()){
		key = cons_getchar();

		switch(key){
			case 's':
				new_pid = DeQ(&free_q);
				if (new_pid == -1){
				
					cons_printf("Panic: no more available process ID left!\n");				

				}else{
					StartProcISR(new_pid);

				}
				break;

			case 'e':
				EndProcISR();
				break;
			case 'b':
				breakpoint();
				break;
			case 'x':
				exit(0);
				break;
			default :
				// no keys were pressed
				break;
		}
	}
	Scheduler();				// to chose process to load/run if needed
	LoadRun(pcb[running_pid].TF_ptr);	// to load/run selected proc
}
