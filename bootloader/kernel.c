/* kernel.c: entry point for C part of kernel. */
#include "etc.h"
#include "textmode.h"

void cmain() {     

	/* Reset terminal so messages can be printed to screen. */           
	resetTerminal();
	putChar('H');
	putChar('e');
	putChar('l');
	putChar('l');
	putChar('o');
	putChar('\n');
	
	putChar('W');
	putChar('o');
	putChar('r');
	putChar('l');
	putChar('d');
	putChar('\n');

	//char text[8] = {'H', 'e', 'l', 'l', 'o', '.', '\n', '\0'};
	const char *text = "Hello.\n";
	putChar(text[0]);
	
    /* hang so CPU doesn't start executing random instructions */
	hang();

} 
