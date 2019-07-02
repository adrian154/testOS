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
	unsigned short limitLower;				/* Lower 16 bits of base.*/
	unsigned short baseLower;				/* Lower 16 bits of base. */
	unsigned char baseMiddle;				/* Middle 16 bits of base. */
	unsigned char access;					/* Access control information. */
	unsigned char limitAndFlags;			/* Upper 4 bits of limit (the limit itself is only 20 bits) and 4 bits of attributes. */
	unsigned char baseUpper;				/* Upper 16 bits of the base address. */
}__attribute__((packed));

struct GDTPointer {
	unsigned short limit;					/* Number of GDT entries.*/
	unsigned int base;						/* Base address of the GDT. */
}__attribute__((packed));								

extern void loadGDT();
extern void installGDT();
extern void installGDTGate(unsigned char index, unsigned long base, unsigned int limit, unsigned char access, unsigned char flags);


