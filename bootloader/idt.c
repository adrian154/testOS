#include "idt.h"
#include "misc.h"

/* Define 256 entries. */
struct IdtEntry idt[256];
struct IdtPointer idtPointer;

/* Install an IDT gate in our IDT. */
void installIdtGate(unsigned char interrupt, unsigned long base, unsigned short selector, unsigned char attributes) {
	idt[interrupt].alwaysZero = 0;
	idt[interrupt].offsetLower = base & 0xFFFF;
	idt[interrupt].offsetUpper = base >> 16;;
	idt[interrupt].attributes = attributes;
	idt[interrupt].selector = selector;
}

/* Install our IDT; see idt.asm for loadIdt(). */
void installIdt() {
	idtPointer.limit = sizeof(struct IdtEntry) * 256 - 1;
	idtPointer.base = (unsigned int)&idt;	

	memset(&idt, 0, sizeof(struct IdtEntry) * 256);
	
	loadIdt();
}


