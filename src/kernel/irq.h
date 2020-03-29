#ifndef __IRQ_H
#define __IRQ_H

#include "idt.h"

#define IRQ_TIMER           0
#define IRQ_PS2_KEYBOARD    1
#define IRQ_PS2_MOUSE       12
#define IRQ_ATA_PRIMARY     14
#define IRQ_ATA_SECONDARY   15

extern void installIRQHandler(unsigned char irq, IRQHandler handler);
extern void clearIRQHandler(unsigned char irq);
extern void installIRQs();

#endif