#ifndef __IDT_H
#define __IDT_H

/* Bitmasks for Attributes. */
#define IDT_TASKGATE 		0b00000101
#define IDT_INTERRUPTGATE 	0b00001110
#define IDT_TRAPGATE 		0b00001111
#define IDT_PRESENT 		0b10000000

struct ExceptionFrame {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int interruptNumber, errorCode;
	unsigned int eip, cs, eflags, userEsp, ss;
};

struct InterruptFrame {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int interruptNumber;
	unsigned int eip, cs, eflags, userEsp, ss;
};

/* Typedef to make function pointers less weird. */
typedef void (*IRQHandler)(struct InterruptFrame *frame);

struct IDTEntry {
	unsigned short offsetLower;				/* Lower 16 bits of offset. */
	unsigned short selector;				/* Selector of interrupt function (kernel selector) */
	unsigned char alwaysZero;				/* Always zero. */
	unsigned char attributes;				/* Interrupt gate type and attributes. */
	unsigned short offsetUpper;				/* Upper 16 bits of offset. */
}__attribute__((packed));

struct IDTPointer {
	unsigned short limit;					/* Length of IDT in bytes (minus one) */
	unsigned int base;						/* Base address of IDT. */			
}__attribute__((packed));

extern void loadIDT();
extern void installIDTGate(unsigned char interrupt, unsigned long base, unsigned short selector, unsigned char attributes);
extern void installIDT();

#endif
