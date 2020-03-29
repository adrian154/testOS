#include "acpi.h"
#include "hpet.h"
#include "misc.h"
#include "textmode.h"

struct HPETDescriptor *hpetDescriptor;

void writeRegister64(uint16 regNumber, uint64 value) {

	uint64 *addr = (uint64 *)(hpetDescriptor->address.address + regNumber * 8);
	*addr = value;

}

uint64 readRegister64(uint16 regNumber) {

	uint64 *addr = (uint64 *)(hpetDescriptor->address.address  + regNumber * 8);
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
	uint64 generalCapabilities = readRegister64(GENERAL_CAPABILITIES);
	uint32 capabilitiesLower = generalCapabilities & 0x00000000FFFFFFFF;
	uint32 period = generalCapabilities >> 32;

	unsigned char numTimers = (capabilitiesLower & 0x00001F00) >> 8;
	bool canDoLegacy = capabilitiesLower & 0x00008000;

	/* Debug messages */
	printString("HPET has "); printInt(numTimers); printString(" comparator(s).\n");
	printString("HPET period is "); printInt(period / 1000000); printString(" nanoseconds.\n");	

	return true;

}
