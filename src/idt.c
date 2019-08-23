#include "idt.h"
#include "misc.h"

/* Define 256 entries. */
struct IDTEntry idt[256];
struct IDTPointer IDTPointer;

/* Install an IDT gate in our IDT. */
void installIDTGate(unsigned char interrupt, unsigned long base, unsigned short selector, unsigned char attributes) {
	idt[interrupt].alwaysZero = 0;
	idt[interrupt].offsetLower = base & 0xFFFF;
	idt[interrupt].offsetUpper = base >> 16;;
	idt[interrupt].attributes = attributes;
	idt[interrupt].selector = selector;
}

/* Install our IDT; see idt.asm for loadIdt(). */
void installIDT() {
	IDTPointer.limit = sizeof(struct IDTEntry) * 256 - 1;
	IDTPointer.base = (unsigned int)&idt;	

	memset(&idt, 0, sizeof(struct IDTEntry) * 256);
	
	loadIDT();
}


