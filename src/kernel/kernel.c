/* kernel.c: entry point for C part of kernel. */
#include "bool.h"
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
#include "serial.h"
#include "ps2.h"
#include "ps2kb.h"
#include "misc.h"
#include "paging.h"

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

	asm("sti");

	/* Dump memory map for debug purposes. */
	printString("parsing memory map...\n");
	
	/* Read memory map. */
	unsigned short numMapEntries = *(unsigned short *)0x85FE;
	printMemoryMap(numMapEntries, (struct MemoryMapEntry *)0x8400);

	/* Enable keyboard. */
	if(!initPS2Controller()) {
		ERROR("fatal: couldn't enable PS/2 controller\n");
		hang();
	}
	printString("initialized PS/2 controller.\n");

	if(!initKeyboard()) {
		ERROR("fatal: could not initialize PS/2 keyboard\n");
	}
	printString("initialized PS/2 keyboard.\n");

	/* Try to locate RSDP. If it cannot be found, print an error. */
	findRSDP();
	if(RSDP == 0) {
		ERROR("fatal: RSDP was missing or corrupted\n");
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
		WARNING("ACPI 2.0 features not supported\n");
	}
	putChar('\n');
	
	/* Initialize RSDT. */
	if(!initRSDT()) {
		ERROR("fatal: RSDT missing or corrupted\n");
		hang();
	}

	/* Try to initialize HPET. */
	if(!initHPET()) {
		ERROR("fatal: could not initialize HPET\n");
		hang();
	}
	printString("initialized HPET\n");

	/* Print kernel space for debug purposes. */
	printString("kernel occupies 0x");
	printDword(kernelPhysicalStart);
	printString(" to 0x");
	printDword(kernelPhysicalEnd);
	putChar('\n');

	initPagingTest();
	printString("enabled paging\n");

	/* Loop forever. */
	for(;;);

}