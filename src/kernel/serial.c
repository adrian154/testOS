#include "serial.h"
#include "bool.h"
#include "misc.h"

/* stripped down serial implementation for debugging. */

void initSerial() {
	
	/* Disable interrupts.*/
	outb(INTERRUPT_ENABLE_REGISTER, 0x00);
	
	/* Set DLAB (MSB of line control register). */
	outb(LINE_CONTROL_REGISTER, DLAB_SET);
	
	/* Switch to 57600 baud. */
	outb(DIVISOR_LOWER_REGISTER, 0x02);
	outb(DIVISOR_HIGHER_REGISTER, 0x00);
	
	/* Set line parameters to 8N1 (8 bits, no parity, 1 stop bit) */
	outb(LINE_CONTROL_REGISTER, _8_DATA | _1_STOP | _NO_PARITY);
	
	/* Enable FIFO??? not sure */
	/* why C7? */
	outb(FIFO_CONTROL_REGISTER, 0xC7);

	/* RTS? DSR?? No idea what this one's for */
	outb(MODEM_CONTROL_REGISTER, 0x0B);
}

bool transmitEmpty() {
	return (inb(LINE_STATUS_REGISTER) & 0x20);
}

void serialWriteChar(char ch) {
	while(transmitEmpty() == false);
	outb(COM1, ch);	
}

void serialWrite(const char *str) {
	do {
		serialWriteChar(*str);
		str++;
	} while(*str != '\0');
}

