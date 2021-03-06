#ifndef __PIT_H
#define __PIT_H

#define PIT_CHANNEL0 		0b00000000
#define PIT_CHANNEL1 		0b01000000
#define PIT_CHANNEL2 		0b10000000

#define PIT_LOBYTE			0b00010000
#define PIT_HIBYTE			0b00100000
#define PIT_LOBYTEHIBYTE	0b00110000

#define PIT_MODE0			0b00000000
#define PIT_MODE1			0b00000010
#define PIT_MODE2			0b00000100
#define PIT_MODE3			0b00000110
#define PIT_MODE4			0b00001000
#define PIT_MODE5			0b00001010

#define PIT_CHANNEL0_DATA	0x40
#define PIT_CHANNEL1_DATA	0x41
#define PIT_CHANNEL2_DATA	0x42
#define PIT_COMMAND			0x43

#define PIT_FREQ			1193182

extern volatile int tick;
extern void installPIT();

#endif