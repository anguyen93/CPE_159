/* sample.c - Sample Main program for SPEDE	Feb 1, bjw */
#include <spede/stdio.h>	/* For printf() */
#include <spede/flames.h>	/* For cons_printf() */

int main(void)
{
	long i;
	
	i = 128;
	printf( "%d Hello world %d \nECS", i, 2 * i);
	cons_printf( "--> Hello world <--\nCPE/CSC" );	/* target printf */
	return 0;
	/* end main()*/
}
