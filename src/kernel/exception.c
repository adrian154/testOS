#include "textmode.h"
#include "idt.h"
#include "gdt.h"
#include "misc.h"
#include "exception.h"

/* Exception handlers defined in exception.asm. */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void installISRs() {
	
	/* Install an IDT entry for each ISR. */
	installIDTGate(0, (unsigned int)isr0, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(1, (unsigned int)isr1, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(2, (unsigned int)isr2, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(3, (unsigned int)isr3, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(4, (unsigned int)isr4, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(5, (unsigned int)isr5, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(6, (unsigned int)isr6, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(7, (unsigned int)isr7, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(8, (unsigned int)isr8, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(9, (unsigned int)isr9, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(10, (unsigned int)isr10, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(11, (unsigned int)isr11, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(12, (unsigned int)isr12, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(13, (unsigned int)isr13, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(14, (unsigned int)isr14, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(15, (unsigned int)isr15, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(16, (unsigned int)isr16, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(17, (unsigned int)isr17, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(18, (unsigned int)isr18, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(19, (unsigned int)isr19, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(20, (unsigned int)isr20, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(21, (unsigned int)isr21, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(22, (unsigned int)isr22, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(23, (unsigned int)isr23, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(24, (unsigned int)isr24, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(25, (unsigned int)isr25, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(26, (unsigned int)isr26, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(27, (unsigned int)isr27, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(28, (unsigned int)isr28, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(29, (unsigned int)isr29, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(30, (unsigned int)isr30, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(31, (unsigned int)isr31, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	
}

/* Lookup table: Find name based on exception number. */
const char *exceptionMessages[32] = {
	"Divide by Zero Exception",
	"Debug Exception",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow Exception",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved 1",
	"x87 Floating Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating Point Exception",
	"Virtualization Exception",
	"Reserved 2",
	"Reserved 3",
	"Reserved 4",
	"Reserved 5",
	"Reserved 6",
	"Reserved 7",
	"Reserved 8",
	"Reserved 9",
	"Reserved 10",
	"Security Exception",
	"Reserved 11"
};

/* Unified fault handler for all exceptions. */
void faultHandler(struct ExceptionFrame *frame) {
	
	/* If the interrupt is an exception, print an error message, dump registers */
	if(frame->interruptNumber < 32) {
		
		terminalBackground = RED;
		clearScreen();

		printString("fatal: cpu exception 0x"); printByte(frame->interruptNumber); printString(": ");
		printString(exceptionMessages[frame->interruptNumber]);
		printString(" (error code 0x"); printDword(frame->errorCode); printString(").\n");
		
		if(frame->interruptNumber == E_PAGE_FAULT) {

			/* debug: cr2 contains page fault illegal access addr */
			/* do something else with this eventually */
			uint32 addr;
			__asm__ __volatile__("mov %%cr2, %0" : "=r" (addr) :: "eax");
			printString("extras: address was 0x"); printDword(addr); putChar('\n');

			if(frame->errorCode & 0b100) {
				printString("user ");
			} else {
				printString("kernel ");
			}
			printString("process tried to ");
			if(frame->errorCode & 0b010) {
				printString("write to ");
			} else {
				printString("read from ");
			}
			if(frame->errorCode & 1) {
				printString("a present page\n");
			} else {
				printString("a non-present page\n");
			}

		}

		terminalForeground = WHITE;

		printString("EAX=0x"); printDword(frame->EAX);
		printString(" EBX=0x"); printDword(frame->EBX);
		printString(" ECX=0x"); printDword(frame->ECX);
		printString(" EDX=0x"); printDword(frame->EDX);
		putChar('\n');
		
		printString("ESI=0x"); printDword(frame->ESI);
		printString(" EDI=0x"); printDword(frame->EDI);
		printString(" ESP=0x"); printDword(frame->userESP);
		printString("  SS=0x"); printDword(frame->userSS);
		putChar('\n');
		
		printString(" DS=0x"); printDword(frame->DS);
		printString("  ES=0x"); printDword(frame->ES);
		printString("  FS=0x"); printDword(frame->FS);
		printString("  GS=0x"); printDword(frame->GS);
		putChar('\n');
		
		printString(" CS=0x"); printDword(frame->CS);
		printString(" EIP=0x"); printDword(frame->EIP);
		printString(" EFLAGS=0x"); printDword(frame->EFLAGS);
		putChar('\n');
		
		printString("(kernel) ESP=0x"); printDword(frame->kernelESP); 
		printString(" EBP=0x"); printDword(frame->kernelEBP);
		putChar('\n');
		
		putChar('\n');
		printString("system halted. manually restart your computer.");
		
		hang();
	
	} else {
	
		/* Print an error message, but resume operation. */
		terminalForeground = BRIGHT_RED;
		printString("nonfatal: exception handler called for non-exception interrupt 0x");
		printByte(frame->interruptNumber);
		printString(", ignoring\n");
		terminalForeground = WHITE;
		printString("resuming system operation.");
	
	}
}
