// proc.c, 159
// processes are coded here

#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions
//#include "main.h"

/*void LoadRun() {         // this is not real
	// if running_pid is 0
	if(running_pid == 0){
		IdleProc();   // as if loads it and it runs
	}else{
		UserProc();
	}
   //else (all other processes)
      //call UserProc()   // as if loads it and it runs
}*/

void IdleProc() {
	for(;;){
		int i;
   		//show msg on PC: "IdleProc (PID 0) runs.\n"
   		//loop 1666667 times calling IO_DELAY()
		cons_printf("IdleProc (PID 0) runs.\n");
   		for(i = 0; i < 1666666; i++){ // IO_DELAY() delays CPU by .65 micro sec
			IO_DELAY();	
		}
		//KernelMain(); // Temp solution
	}
}

void UserProc() {
	
	for(;;){
		int i;
   	//show msg on target PC:
      		//cons_printf("UserProc (PID %d) runs.\n", running_pid);
   	//loop 1666667 times calling IO_DELAY()*/
		cons_printf("UserProc (PID %d) runs.\n", running_pid);
		for(i = 0; i < 1666666; i++){
			IO_DELAY();
		}
		//KernelMain(); // Temp solution	
	}
}
