#ifndef __IDT_H
#define __IDT_H

#include "types.h"

/* Bitmasks for Attributes. */
#define IDT_TASKGATE 		0b00000101
#define IDT_INTERRUPTGATE 	0b00001110
#define IDT_TRAPGATE 		0b00001111
#define IDT_PRESENT 		0b10000000

struct ExceptionFrame {
	uint32 GS, FS, ES, DS;
	uint32 EDI, ESI, kernelEBP, kernelESP, EBX, EDX, ECX, EAX;
	uint32 interruptNumber, errorCode;
	uint32 EIP, CS, EFLAGS, userESP, userSS;
};

struct InterruptFrame {
	uint32 GS, FS, ES, DS;
	uint32 EDI, ESI, kernelEBP, kernelESP, EBX, EDX, ECX, EAX;
	uint32 interruptNumber;
	uint32 EIP, CS, EFLAGS, userESP, userSS;
};

/* Typedef to make function pointers less weird. */
typedef void (*IRQHandler)(struct InterruptFrame *frame);

struct IDTEntry {
	uint16 offsetLower;				/* Lower 16 bits of offset. */
	uint16 selector;				/* Selector of interrupt function (kernel selector) */
	uint8 alwaysZero;				/* Always zero. */
	uint8 attributes;				/* Interrupt gate type and attributes. */
	uint16 offsetUpper;				/* Upper 16 bits of offset. */
}__attribute__((packed));

struct IDTPointer {
	uint16 limit;					/* Length of IDT in bytes (minus one) */
	uint32 base;						/* Base address of IDT. */			
}__attribute__((packed));

extern void loadIDT();
extern void installIDTGate(uint8 interrupt, uint32 base, uint16 selector, uint8 attributes);
extern void installIDT();

#endif