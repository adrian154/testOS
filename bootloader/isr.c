#include "textmode.h"
#include "idt.h"
#include "gdt.h"
#include "misc.h"

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
	installIdtGate(0, (unsigned int)isr0, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(1, (unsigned int)isr1, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(2, (unsigned int)isr2, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(3, (unsigned int)isr3, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(4, (unsigned int)isr4, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(5, (unsigned int)isr5, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(6, (unsigned int)isr6, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(7, (unsigned int)isr7, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(8, (unsigned int)isr8, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(9, (unsigned int)isr9, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(10, (unsigned int)isr10, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(11, (unsigned int)isr11, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(12, (unsigned int)isr12, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(13, (unsigned int)isr13, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(14, (unsigned int)isr14, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(15, (unsigned int)isr15, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(16, (unsigned int)isr16, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(17, (unsigned int)isr17, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(18, (unsigned int)isr18, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(19, (unsigned int)isr19, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(20, (unsigned int)isr20, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(21, (unsigned int)isr21, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(22, (unsigned int)isr22, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(23, (unsigned int)isr23, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(24, (unsigned int)isr24, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(25, (unsigned int)isr25, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(26, (unsigned int)isr26, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(27, (unsigned int)isr27, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(28, (unsigned int)isr28, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(29, (unsigned int)isr29, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(30, (unsigned int)isr30, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIdtGate(31, (unsigned int)isr31, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
}

const char *exceptionMessages[31] = {
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
	"Reserved",
	"x87 Floating Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating Point Exception",
	"Virtualization Exception",
	"Reserved",
	"Security Exception",
	"Reserved"
};

void faultHandler(struct InterruptFrame *frame) {
	if(frame->interruptNumber < 32) {
		terminalForeground = 0x0C;
		
		putChar('\n');
		printString("exception 0x"); printByte(frame->interruptNumber); printString(": ");
		printString(exceptionMessages[frame->interruptNumber]);
		printString(" (error code 0x"); printDword(frame->errorCode); printString(").\n");
		
		terminalForeground = 0x0F;
		
		printString("EAX="); printDword(frame->eax);
		printString(" EBX="); printDword(frame->ebx);
		printString(" ECX="); printDword(frame->ecx);
		printString(" EDX="); printDword(frame->edx);
		putChar('\n');
		printString("ESI="); printDword(frame->esi);
		printString(" EDI="); printDword(frame->edi);
		printString(" ESP="); printDword(frame->esp);
		printString(" EBP="); printDword(frame->ebp);
		putChar('\n');
		printString("(user) EIP="); printDword(frame->eip);
		printString(" CS="); printDword(frame->cs);
		printString(" SS="); printDword(frame->ss);
		printString(" ESP="); printDword(frame->userEsp);
		putChar('\n');
		printString("EFLAGS="); printDword(frame->eflags);
		putChar('\n');
		
		putChar('\n');
		printString("system halted. manually restart your computer.");
		
		hang();
	}
}
