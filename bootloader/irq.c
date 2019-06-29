#include "irq.h"
#include "idt.h"
#include "pic.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();



/* 16 IRQ handlers for each IRQ. */
IRQHandler IRQHandlers[16];

/* Install an IRQ handler. */
void installIRQHandler(unsigned int irq, IRQHandler handler) {
	IRQHandlers[irq] = handler;
}

/* Clear an IRQ handler. */
void clearIRQHandler(unsigned int irq) {
	IRQHandlers[irq] = (IRQHandler)0;
}

/* Remap IRQs; by default IRQ0-7 are mapped to interrupts 8-15 which won't work for us because this coincides with the CPU's built in exceptions. */
void installIRQs() {
	remapPIC(0x20, 0x28);
	installIDTGate(32, irq0)
}
