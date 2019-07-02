#include "misc.h"
#include "pic.h"
#include "textmode.h"

void remapPIC(unsigned char offset1, unsigned char offset2) {
	
	printString("remapping pics to offsets 0x"); printByte(offset1); printString(" and 0x"); printByte(offset2);
	printString(".\n");
	
	/* Send 1st initialization word. 0x11 specifies that a 4th ICW will also be sent. */
	outb(PIC1_COMMAND, 0x11);
	outb(PIC2_COMMAND, 0x11);
	
	/* Send offset. */
	outb(PIC1_DATA, offset1);
	outb(PIC2_DATA, offset2);
	
	/* Tell master that slave is on IRQ2 (ICW bits 3,4) and tell slave that they are to cascade on IRQ2. */
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	
	/* ??? */
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);
	
	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);
}

void testRemap() {
	outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void sendEOI(unsigned char irq) {
	
	/* If IRQ 8-15, PIC #2 also needs to receive an EOI. */
	if(irq >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
	
	/* Regardless, send EOI to PIC1. */
	outb(PIC1_COMMAND, PIC_EOI);
	
}
