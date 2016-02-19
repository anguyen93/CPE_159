// ********************************************************
// Name: Anh Nguyen, Joseph Gonzalez
// Team Name: Decepticons 
// isr.c, 159
// ********************************************************

#include "spede.h"
#include "typedef.h"
#include "isr.h"
#include "toolfunc.h"
#include "extern.h"
#include "proc.h"

void StartProcISR(int new_pid) {
   	//clear the PCB of the new pid
   	MyBzero((char *) &pcb[new_pid], sizeof(pcb_t));
 
   	//set its state to READY
	pcb[new_pid].state = READY;
 
   	//if new pid is not 0 (IdleProc),
      	//then, enqueue this new pid into the ready queue*/
      	if(new_pid != 0){
		EnQ(new_pid, &ready_q);
	}
 	
	//Clears the stack
	MyBzero((char *) &proc_stack[new_pid], PROC_STACK_SIZE);

	//Set TF_ptr of PCB close to end (top) of stack, then fill out
	pcb[new_pid].TF_ptr = (TF_t *) &proc_stack[new_pid][PROC_STACK_SIZE];
	pcb[new_pid].TF_ptr--;

	pcb[new_pid].TF_ptr->eflags = EF_DEFAULT_VALUE|EF_INTR; // set INTR flag
	pcb[new_pid].TF_ptr->cs = get_cs();			// standard fair
	pcb[new_pid].TF_ptr->ds = get_ds();			// standard fair
	pcb[new_pid].TF_ptr->es = get_es();			// standard fair
	pcb[new_pid].TF_ptr->fs = get_fs();			// standard fair
	pcb[new_pid].TF_ptr->gs = get_gs();			// standard fair

	if(new_pid == 0){
		pcb[new_pid].TF_ptr->eip = (unsigned int) IdleProc; // if pid is 0, points to IdleProc 
	}else{
		pcb[new_pid].TF_ptr->eip = (unsigned int) UserProc; // or UserProc
	}
}

void EndProcISR() {
   	//if running PID is 0 (IdleProc should not let exit),
      	//then, just return;

	if(running_pid == 0){
					
		return;
			
	}else{ // Change the state to FREE, q the running pid into the free_q, and set running pid to -1/
		pcb[running_pid].state = FREE;
		EnQ(running_pid,&free_q);
		running_pid = -1;
	}

}        

void TimerISR() {
   	//just return if running PID is -1 (not any valid PID)
	
	if(running_pid== -1){
		cons_printf("PANIC MESSAGE: RUNNING PID = -1\n");
		return;
	
	}else{
		//in PCB, upcount both runtime and total_runtime of running process

		pcb[running_pid].runtime = pcb[running_pid].runtime+1;
		pcb[running_pid].total_runtime = pcb[running_pid].total_runtime+1;
	
		if(pcb[running_pid].runtime== TIME_LIMIT){ // If runtime has reached TIME_LIMIT
			// Reset runtime
			// Change the state to READY
			// queue to ready_q
			// Set pid to -1

			pcb[running_pid].runtime = 0;
			pcb[running_pid].state = READY;
			EnQ(running_pid, &ready_q);
			running_pid = -1;
		}	
	}
}
