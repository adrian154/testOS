#ifndef __GDT_H
#define __GDT_H

#include "types.h"

#define GDT_CODE_DESCRIPTOR 0x08
#define GDT_DATA_DESCRIPTOR 0x10

/* Bitmasks for Access byte. */
#define GDT_PRESENT			0b10000000
#define GDT_RING0			0b00000000
#define GDT_RING1			0b00100000
#define GDT_RING2			0b01000000
#define GDT_RING3			0b01100000
#define GDT_IS_CODE_OR_DATA 0b00010000
#define GDT_EXECUTABLE		0b00001000
#define GDT_READABLE		0b00000010
#define GDT_WRITABLE		0b00000010

/* Bitmasks for Flags. */
#define GDT_IS_4K			0b10000000
#define GDT_IS_PM			0b01000000

struct GDTEntry {
	uint16 limitLower;				/* Lower 16 bits of limit.*/
	uint16 baseLower;				/* Lower 16 bits of base. */
	uint8 baseMiddle;				/* Middle 16 bits of base. */
	uint8 access;					/* Access control information. */
	uint8 limitAndFlags;			/* Upper 4 bits of limit (the limit itself is only 20 bits since GDT has 4K granularity) and 4 bits of attributes. */
	uint8 baseUpper;				/* Upper 16 bits of the base address. */
}__attribute__((packed));

struct GDTPointer {
	uint16 limit;					/* Number of GDT entries.*/
	uint32 base;					/* Base address of the GDT. */
}__attribute__((packed));								

extern void loadGDT();
extern void installGDT();
extern void installGDTGate(uint8 index, uint32 base, uint32 limit, uint8 access, uint8 flags);

#endif