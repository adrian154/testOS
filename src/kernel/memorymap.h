#ifndef __MEMORYMAP_H
#define __MEMORYMAP_H

struct MemoryMapEntry {
	unsigned int baseLower;
	unsigned int baseUpper;
	unsigned int lengthLower;
	unsigned int lengthUpper;
	unsigned int type;
	unsigned int ACPI;
}__attribute__((packed));

extern void printMemoryMap(unsigned short numEntries, struct MemoryMapEntry *map);

#endif