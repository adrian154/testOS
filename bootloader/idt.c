#include "idt.h"

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
};
