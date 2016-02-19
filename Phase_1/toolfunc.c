// toolfunc.c, 159
// CPE 159
// Team Name: Decepticons (Members: Anh Nguyen, Joseph Gonzalez)

#include "spede.h"
#include "typedef.h"
#include "extern.h"

void MyBzero(char *p, int q_len) {
	// clears queue
	int i;
 
	for(i = 0; i < q_len; i++){
		*p++ = 0;
	}
}

void EnQ(int data, q_t *q_addr) {
	
	if(q_addr->len == Q_LEN){ // If q is full, return return message
		cons_printf("Enqueue is already full\n");
		return;
	}

	q_addr-> q[q_addr->tail]= data;
	q_addr-> tail++;
	q_addr-> len++;
	
	if(q_addr->tail == Q_LEN){ // if q's tail is full, then reset tail
		q_addr->tail = 0;
	}
}

int DeQ(q_t *ptr) { // return -1 if q is empty
	
//If the length is already 0 for that q struct	
	int data = 0 ;

	if(ptr->len == 0){ // If the q is empty, return -1
		return -1;
	}else{
	
	// pull the data at that position and try to return it
		data = ptr-> q[ptr->head];
	// since we have dequed the data we need to shift the head and dec length also check for wrapping
		ptr->head++;
		ptr->len--;
	
	// Check to see if we have made head small than the Q
	// If we try to dec the head past 0 push it back up to 0
		if(ptr->head == Q_LEN){
			ptr->head = 0 ;
		}
	}
	return data;
}
