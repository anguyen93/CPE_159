// proc.c, 159
// processes are coded here
// Team Name: Decepticons (Memebers: Anh Nguyen, Josepth Gonzalez)

#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions

void IdleProc() {
	for(;;){
		int i;
		cons_printf("IdleProc (PID 0) runs.\n");
   		for(i = 0; i < 1666666; i++){ // IO_DELAY() delays CPU
			IO_DELAY();	
		}
		
	}
}

void UserProc() {
	for(;;){
		int i;
		cons_printf("UserProc (PID %d) runs.\n", running_pid);
		for(i = 0; i < 1666666; i++){ // IO_DELAY() delays CPU
			IO_DELAY();
		}
			
	}
}
