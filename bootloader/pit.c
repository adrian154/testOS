#include "pit.h"
#include "idt.h"
#include "irq.h"
#include "textmode.h"
#include "misc.h"

volatile int tick;

void PITHandler(struct InterruptFrame *frame) {
	tick++;
}

void setPITDivisor(unsigned short divisor) {
	outb(PIT_COMMAND, PIT_CHANNEL0 | PIT_LOBYTEHIBYTE | PIT_MODE3);
	outb(PIT_CHANNEL0_DATA, divisor & 0xFF);
	outb(PIT_CHANNEL0_DATA, (divisor >> 8) & 0xFF);
}

void setPITFrequency(unsigned int frequency) {
	unsigned int divisor = PIT_FREQ / frequency;
	
	if(divisor > 0xFFFF) {
		terminalForeground = BRIGHT_YELLOW;
		printString("warning: PIT frequency truncated to 18 Hz because of hardware limitations.\n");
		terminalForeground = WHITE;
		divisor = 0xFFFF;
	}
	
	setPITDivisor((unsigned short)divisor & 0xFFFF);
}

void installPIT() {
	installIRQHandler(0, PITHandler);
	tick = 0;
	setPITFrequency(1000);
}