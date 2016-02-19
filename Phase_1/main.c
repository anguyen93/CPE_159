// main.c, 159
// the kernel is simulated, not yet real
//
// Team Name: Decepticons (Members: Anh Nguyen and Joseph Gonzalez)

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
	
	//Sets kernel data.
	InitKernelData();
	
	//Sets kernel control.
	InitKernelControl();

	//Dequeue a PID from the free_Q
	running_pid = DeQ(&free_q);
	
	//Create an idle process with this PID
	StartProcISR(running_pid);
	
	LoadRun(pcb[0].TF_ptr); // load/run IdleProc	

	return 0; // End of main
}

void InitKernelData() {
   	int i;

   	//call MyBzero() to clear queues (which is to be coded in toolfunc.h/.c)
	MyBzero((char *) &ready_q, MAX_PROC_NUM);
	MyBzero((char *) &free_q, MAX_PROC_NUM);
	
	// lopp number i from 0 to 19
   	for(i = 0; i < Q_LEN; i++){
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
}

void Scheduler() {  // to choose running PID
	
	if(running_pid > 0){ // If running pid is 0, just return
		return;
	}else if(running_pid == 0){ // Change the state to READY
		pcb[running_pid].state = READY;	
		
	}

	running_pid= DeQ(&ready_q); // Get pid from the free_q

	if(running_pid == -1){ // If running pid is -1, set it to 0
		running_pid= 0;		
	}

	pcb[running_pid].state = RUN; // Change the state to RUN
}

void KernelMain(TF_t *TF_ptr) {
   int new_pid;
   char key;

	pcb[running_pid].TF_ptr = TF_ptr; // Save TF_ptr to PCB of running process

	switch(TF_ptr->intr_id){
		case TIMER_INTR:
			TimerISR();
			outportb(0x20,0x60); // Dismiss timer event: send PIC with a code
			break;
		default: 
			cons_printf("Panic: unknown intr ID (%d)!\n", TF_ptr->intr_id);
			breakpoint();	// fallback to GDB 
	}

	if(cons_kbhit()){
		key = cons_getchar();

		switch(key){
			case 's':
				new_pid = DeQ(&free_q); // Gets pid from the free_q
				if (new_pid == -1){ // If pid is -1, then show a panic message
					cons_printf("Panic: no more available process ID left!\n");				
				}else{ // Create a new process
					StartProcISR(new_pid); 
				}
				break;

			case 'e':
				EndProcISR(); // Ends the process
				break;
			case 'b':
				breakpoint(); // Goes to GDB
				break;
			case 'x':
				exit(0); // Quits program
				break;
			default :
				// no keys were pressed
				break;
		}
	}

	Scheduler();				// to chose process to load/run if needed
	LoadRun(pcb[running_pid].TF_ptr);	// to load/run selected proc
}
