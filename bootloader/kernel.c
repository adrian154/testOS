/* kernel.c: entry point for C part of kernel. */
#include "etc.h"
#include "textmode.h"

void cmain() {     

	/* Reset terminal so messages can be printed to screen. */           
	resetTerminal();
	
	/* Finally! */
	printString("Hello, world! For good!\n");
		
    /* Hang so CPU doesn't start executing random instructions. */
	hang();

} 
