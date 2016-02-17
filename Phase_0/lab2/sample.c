/* sample.c - Sample Main program for SPEDE	Feb 1, bjw */
#include <spede/stdio.h>	/* For printf() */
#include <spede/flames.h>	/* For cons_printf() */

int main(void)
{
	int i, j;
	
	i = 128;
	for(j = 0; j < 5; j++){
		DisplayMsg(i); // diplays the messages
		i++;
	}
	return 0;
	/* end main()*/
}

void DisplayMsg(int i){
	printf( "%d Hello world %d \nECS", i, 2 * i);
	cons_printf( "--> Hello world <--\nCPE/CSC" );  /* target pr    intf */
}
