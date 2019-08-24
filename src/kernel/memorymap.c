#include "memorymap.h"
#include "textmode.h"

extern void printMemoryMap(unsigned short numEntries, struct MemoryMapEntry *map) {
	unsigned int free = 0;
	unsigned int used = 0;
	
	for(unsigned short i = 0; i < numEntries; i++) {
		
		struct MemoryMapEntry *entry = map;
		printString("entry 0x"); printByte(i);
		printString(": base=0x"); printDword(entry->baseUpper); printDword(entry->baseLower);
		printString(" length=0x"); printDword(entry->lengthUpper); printDword(entry->lengthLower);
		printString(" type=0x"); printByte(entry->type);
		putChar('\n');

		if(entry->type != 1) {
			used += entry->lengthLower;
		} else {
			free += entry->lengthLower;
		}

		map = map + 1;
	}
	
	printString("free=0x"); printDword(free); printString(" reserved=0x"); printDword(used); putChar('\n');
}
