/* kernel.c: entry point for C part of kernel. */
#include "misc.h"
#include "textmode.h"
#include "gdt.h"
#include "idt.h"
#include "exception.h"
#include "memorymap.h"
#include "irq.h"

void testHandler(struct InterruptFrame *frame) {
	putChar('a');
}

void cmain() {     

	/* Reset terminal so messages can be printed to screen. */           
	resetTerminal();
	
	/* Finally! */
	terminalForeground = BRIGHT_GREEN;
	printString("testOS kernel started.\n");
	terminalForeground = WHITE;
	
	/* Despite there already being a valid (and identical) GDT, just set one up in C for the sake of it. */
	installGDT();
	printString("installed new GDT.\n");
	
	/* Set up an IDT so we can install our own interrupt handlers. */
	installIDT();
	printString("installed IDT.\n");
	
	/* Install handlers for interrupst 0-31 (CPU-generated exceptions. */
	installISRs();
	printString("installed exception handler.\n");
	
	/* Set up IRQs. */
	installIRQs();
	printString("installed IRQ handler.\n");
	
	/* Dump memory map. */
	printString("parsing memory map...\n");
	unsigned short numMapEntries = *(unsigned short *)0x83FE;
	printMemoryMap(numMapEntries, (struct MemoryMapEntry *)0x8200);
	
	installIRQHandler(1, testHandler);
	asm("sti");
	asm("int $0x20");

    /* Hang so CPU doesn't start executing random instructions. */
	hang();

} 
