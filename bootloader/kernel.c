/* kernel.c: entry point for C part of kernel. */
#include "misc.h"
#include "textmode.h"
#include "idt.h"
#include "isr.h"

void cmain() {     

	/* Reset terminal so messages can be printed to screen. */           
	resetTerminal();
	
	/* Finally! */
	terminalForeground = 0xA;
	printString("testOS kernel started.\n");
	terminalForeground = 0xF;
	
	installIdt();
	printString("installed IDT.\n");
	
	installISRs();
	printString("installed ISRs.\n");
	
    /* Hang so CPU doesn't start executing random instructions. */
	hang();

} 
