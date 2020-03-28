#include "gdt.h"
#include "misc.h"
#include "textmode.h"

/* Define three entries. */
struct GDTEntry gdt[3];
struct GDTPointer GDTPointer;

/* Install a GDT gate in our GDT. */
void installGDTGate(uint8 index, uint32 base, uint32 limit, uint8 access, uint8 flags) {
	
	/* Set up the base first. */
	gdt[index].baseLower = base & 0xFFFF;
	gdt[index].baseMiddle = (base >> 16) & 0xFF;
	gdt[index].baseUpper = (base >> 24) & 0xFF;

	/* Next, set up the descriptor. */
	gdt[index].limitLower = limit & 0xFFFF;
	gdt[index].limitAndFlags = (limit >> 16) & 0x0F;	
	gdt[index].limitAndFlags |= flags; 

	/* Finally, also add the access byte. */
	gdt[index].access = access;
	
}

/* Install a code and data descriptor. */
void installGDT() {
	
	/* Set up the GDT pointer first. */
	GDTPointer.limit = (sizeof(struct GDTEntry) * 3) - 1;
	GDTPointer.base = (unsigned int)&gdt;
	
	/* Null descriptor. */
	installGDTGate(0, 0, 0, 0, 0);
	
	/* Kernel code, data descriptor. */
	installGDTGate(1, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RING0 | GDT_IS_CODE_OR_DATA | GDT_EXECUTABLE | GDT_READABLE, GDT_IS_4K | GDT_IS_PM);
	installGDTGate(2, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RING0 | GDT_IS_CODE_OR_DATA | GDT_WRITABLE, GDT_IS_4K | GDT_IS_PM);

	/* Get rid of old GDT (set up by the bootloader) and install new changes. */
	loadGDT();
	
}


