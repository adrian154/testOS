#include "acpi.h"
#include "hpet.h"
#include "misc.h"
#include "textmode.h"

bool initHPET() {
	char HPET_SIGNATURE[4] = {'H', 'P', 'E', 'T'};

	/* Try to find HPET ACPI table. If it can't be found, it isn't supported. */
	struct SDTHeader *header = findTable(HPET_SIGNATURE);
	if(header == null) {

		terminalForeground = BRIGHT_RED;
		printString("initHpet(): could not find HPET ACPI table, may not be supported.\n");
		return false;
	}

	

	return true;
}
