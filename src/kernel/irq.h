#ifndef __IRQ_H
#define __IRQ_H

#include "idt.h"

extern void installIRQHandler(unsigned char irq, IRQHandler handler);
extern void clearIRQHandler(unsigned char irq);
extern void installIRQs();

#endif