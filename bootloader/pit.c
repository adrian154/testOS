#include "pit.h"
#include "idt.h"
#include "irq.h"
#include "textmode.h"
#include "misc.h"

void PITHandler(struct InterruptFrame *frame) {

}

void installPIT() {
	installIRQHandler(0, PITHandler);
	outb(PIT_COMMAND, PIT_CHANNEL0 | PIT_LOBYTEHIBYTE | PIT_MODE3);
	outb(PIT_COMMAND, 0x01);
	outb(PIT_COMMAND, 0x00);
}