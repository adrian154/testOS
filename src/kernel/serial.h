#ifndef __SERIAL_H
#define __SERIAL_H

#define COM1 0x3F8

#define DATA_REGISTER 				0x3F8
#define INTERRUPT_ENABLE_REGISTER	0x3F9
#define DIVISOR_LOWER_REGISTER		0x3F8
#define DIVISOR_HIGHER_REGISTER		0x3F9
#define FIFO_CONTROL_REGISTER		0x3FA
#define LINE_CONTROL_REGISTER		0x3FB
#define MODEM_CONTROL_REGISTER		0x3FC
#define LINE_STATUS_REGISTER		0x3FD
#define MODEM_STATUS_REGISTER		0x3FE
#define SCRATCH_REGISTER			0x3FF

#define DLAB_SET	0x80
#define DLAB_UNSET	0x00

#define CLOCK	115200

#define _5_DATA	0x00
#define _6_DATA	0x01
#define _7_DATA 0x02
#define _8_DATA 0x03

#define _1_STOP 0x00
#define _2_STOP 0x04

#define _NO_PARITY		0x00
#define _ODD_PARITY		0x08
#define _EVEN_PARITY	0x18
#define _MARK_PARITY	0x28
#define _SPACE_PARITY	0x38

extern void initSerial();
extern void serialWriteChar(char ch);
extern void serialWrite(const char *str);

#endif