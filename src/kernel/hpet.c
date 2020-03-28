#include "acpi.h"
#include "hpet.h"
#include "misc.h"
#include "textmode.h"

struct HPETDescriptor *hpetDescriptor;

void writeRegister64(unsigned short regNumber, unsigned long value) {

	unsigned long *addr = (unsigned long *)(hpetDescriptor->address.address + regNumber * 4);
	*addr = value;

}

long readRegister64(unsigned short regNumber) {

	unsigned long *addr = (unsigned long *)(hpetDescriptor->address.address  + regNumber * 4);
	return *addr;

}

bool initHPET() {
	
	char HPET_SIGNATURE[4] = {'H', 'P', 'E', 'T'};

	/* Try to find HPET ACPI table. If it can't be found, it isn't supported. */
	struct HPETDescriptor *descriptor = (struct HPETDescriptor *)findTable(HPET_SIGNATURE);
	if(descriptor == null) {

		terminalForeground = BRIGHT_RED;
		printString("initHpet(): could not find HPET ACPI table, may not be supported.\n");
		return false;
	}

	hpetDescriptor = descriptor;
	
	/* Read general capabilities. */
	unsigned long generalCapabilities = readRegister64(GENERAL_CAPABILITIES);
	unsigned int capabilitiesLower = generalCapabilities & 0x00000000FFFFFFFF;
	unsigned int capabilitiesUpper = generalCapabilities & 0xFFFFFFFF00000000;

	unsigned char numTimers = (capabilitiesLower & 0x00001F00) >> 8;
	bool canDoLegacy = capabilitiesLower & 0x00008000;

	printDword(capabilitiesLower); printDword(capabilitiesUpper);
	printString("Timer period in femtoseconds is 0x"); printDword(capabilitiesUpper); printString(" and has 0x"); printByte(numTimers); printString(" timers.");

	return true;

}
