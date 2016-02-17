// example.c
//
// use given SPEDE cons_xxx helper functions

#include <spede/stdio.h>  // cons_xxx to debug runtime
#include <spede/flames.h> // IO_DELAY() macro "inb 0x80"

int main() {
   int i;
   char ch;

   printf("Hello, World! (From developer Linux computer.)\n");

   cons_printf("Hello, World, too! (From development target computer.)\n");

   cons_putchar('W'); // cons_putchar() prints one character
   cons_putchar('X');
   cons_putchar('Y');

   while(1) {
      if( cons_kbhit() ) {                    // poll keyboard, returns 1 if pressed
         ch = cons_getchar();                 // read in pressed key
         break; // break the while loop
      }
      else {
         for(i=0; i<1666000; i++) IO_DELAY(); // .65us each, about 1 sec all
         cons_putchar('Z');
      }
   }

   return 0;
}
