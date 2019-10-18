/* kernel.c: entry point for C part of kernel. */
#include "misc.h"
#include "textmode.h"
#include "gdt.h"
#include "idt.h"
#include "exception.h"
#include "memorymap.h"
#include "irq.h"
#include "acpi.h"
#include "hpet.h"
#include "pit.h"
#include "paging.h"
#include "serial.h"

/* Test: Fired every keypress */
void testHandler(struct InterruptFrame *frame) {
	
	/* (Temporary keypress handler) */
	/* Just read scancode for now */
	inb(0x60);
}

void cmain(unsigned int kernelPhysicalStart, unsigned int kernelPhysicalEnd) {     

	/* Reset terminal so messages can be printed to screen. */           
	resetTerminal();
	
	/* Print debug message. */
	terminalForeground = BRIGHT_GREEN;
	printString("testOS kernel started.\n");
	terminalForeground = WHITE;
	
	/* Set up new GDT in C. */
	installGDT();
	printString("installed new GDT.\n");
	
	/* Set up an IDT so we can install our own interrupt handlers. */
	installIDT();
	printString("installed IDT.\n");
	
	/* Install handlers for interrupst 0-31 (CPU-generated exceptions). */
	installISRs();
	printString("installed exception handler.\n");
	
	/* Set up serial ports. */
	initSerial();
	printString("initialized COM1.\n");
	
	/* Set up IRQs. */
	installIRQs();
	printString("installed IRQ handler.\n");
	
	/* Dump memory map. */
	printString("parsing memory map...\n");
	
	/* Read memory map. */
	unsigned short numMapEntries = *(unsigned short *)0x85FE;
	printMemoryMap(numMapEntries, (struct MemoryMapEntry *)0x8400);

	/* Enable interrupts. */
	asm("sti");
	
	/* Install test handler for keyboard IRQ. */
	installIRQHandler(1, testHandler);
	
	/* Try to locate RSDP. If it cannot be found, print an error. */
	findRSDP();
	if(RSDP == 0) {
		terminalForeground = BRIGHT_RED;
		printString("fatal: could not find RSDP or RSDP is corrupted.\n");
		terminalForeground = WHITE;
	
		printString("system halted. manually restart your computer.");
		hang();
	}
	
	/* Print some debug messages. */
	printString("found RSDP at 0x"); printDword((unsigned int)RSDP);
	putChar('\n');
	
	/* Print OEMID for the sake of it. */
	printString("OEMID is \"");
	for(unsigned int i = 0; i < 6; i++) {
		putChar(RSDP->OEMID[i]);
	}
	printString("\"\n");
	
	/* Warning, because ACPI 2.0 may cause some weirdness on dodgy machines. */
	printString("ACPI revision is 0x"); printByte(RSDP->revision);
	if(RSDP->revision > 0) {
		terminalForeground = BRIGHT_YELLOW;
		printString("warning: testOS does not support ACPI 2.0+\n");
		terminalForeground = WHITE;
	}
	putChar('\n');
	
	/* Initialize RSDT. */
	if(!initRSDT()) {
		terminalForeground = BRIGHT_RED;
		printString("fatal: could not initialize RSDT or RSDT is corrupted.");
		hang();
	}

	/* Try to initialize HPET. */
	if(!initHPET()) {
		terminalForeground = BRIGHT_RED;
		printString("fatal: could not initialize HPET");
		hang();
	}
	
	/* Print kernel space for debug purposes. */
	printString("kernel occupies ");
	printDword(kernelPhysicalStart);
	printString(" to ");
	printDword(kernelPhysicalEnd);
	putChar('\n');
	
	/* Set up paging. */
	setupPaging();
	printString("enabled paging.\n");
	
	char *ptr = 0xDEADBEEF;
	*ptr = 0;
	
	/* Well, I'm stuck. */
	/* I would work on writing a page frame allocator so I can get to work on context switching, userspace, and eventually system calls. */
	/* However, I have no ambition and the mounting school stress I'm experiencing is super unmotivating. */
	/* I'm sorry for the probable lack of commits after this. I have nowhere to go. */
	/* Hang forever. */
	
	for(;;);
} 
