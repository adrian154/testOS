/* PIC command and data ports. */
#define PIC1_COMMAND	0x20
#define PIC1_DATA		0x21
#define PIC2_COMMAND	0xA0
#define PIC2_DATA		0xA1

/* PIC command: EOI */
#define PIC_EOI			0x20

extern void remapPIC(unsigned char offset1, unsigned char offset2);
