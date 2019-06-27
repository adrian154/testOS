#define IDT_TASKGATE 0b0101
#define IDT_INTERRUPTGATE 0b1110
#define IDT_TRAPGATE 0b1111
#define IDT_PRESENT 0b10000000

struct InterruptFrame {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int interruptNumber, errorCode;
	unsigned int eip, cs, eflags, userEsp, ss;
};

struct IdtEntry {
	unsigned short offsetLower;				/* Lower 16 bits of offset. */
	unsigned short selector;				/* Selector of interrupt function (kernel selector) */
	unsigned char alwaysZero;				/* Always zero. */
	unsigned char attributes;				/* Interrupt gate type and attributes. */
	unsigned short offsetUpper;				/* Upper 16 bits of offset. */
}__attribute__((packed));

struct IdtPointer {
	unsigned short limit;					/* Length of IDT in bytes (minus one) */
	unsigned int base;						/* Base address of IDT. */			
}__attribute__((packed));


extern void loadIdt();
extern void installIdtGate(unsigned char interrupt, unsigned long base, unsigned short selector, unsigned char attributes);
extern void installIdt();
