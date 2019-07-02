#include "irq.h"
#include "idt.h"
#include "gdt.h"
#include "pic.h"
#include "textmode.h"

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
void installIRQHandler(unsigned char irq, IRQHandler handler) {
	IRQHandlers[irq] = handler;
}

/* Clear an IRQ handler. */
void clearIRQHandler(unsigned char irq) {
	IRQHandlers[irq] = (IRQHandler)0;
}

/* Remap IRQs; by default IRQ0-7 are mapped to interrupts 8-15 which won't work for us because this coincides with the CPU's built in exceptions. */
void installIRQs() {
	//remapPIC(0x20, 0x28);
	testRemap();
	
	installIDTGate(32, (unsigned int)irq0, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(33, (unsigned int)irq1, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(34, (unsigned int)irq2, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(35, (unsigned int)irq3, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(36, (unsigned int)irq4, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(37, (unsigned int)irq5, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(38, (unsigned int)irq6, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(39, (unsigned int)irq7, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(40, (unsigned int)irq8, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(41, (unsigned int)irq9, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(42, (unsigned int)irq10, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(43, (unsigned int)irq11, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(44, (unsigned int)irq12, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(45, (unsigned int)irq13, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(46, (unsigned int)irq14, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);
	installIDTGate(47, (unsigned int)irq15, GDT_CODE_DESCRIPTOR, IDT_PRESENT | IDT_INTERRUPTGATE);

	/* Clear the handlers so we don't execute junk data. */
	for(unsigned int i = 0; i < 16; i++) {
		clearIRQHandler(i);
	}
}

void irqHandler(struct InterruptFrame *frame) {
	unsigned char irq = frame->interruptNumber - 0x20;
	
	/* If a handler is defined for that IRQ, run it. */
	IRQHandler handler = IRQHandlers[irq];
	if(handler) {
		handler(frame);
	}
	
	/* Send EOI to end interrupt. */
	sendEOI(irq);
	
}
