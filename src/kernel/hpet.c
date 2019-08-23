#include "acpi.h"
#include "hpet.h"

bool initHPET() {
	char HPET_SIGNATURE[4] = {'H', 'P', 'E', 'T'};

	struct SDTHeader *header = findTable(HPET_SIGNATURE);
	
	return true;
}
