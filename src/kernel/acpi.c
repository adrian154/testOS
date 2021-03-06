#include "acpi.h"
#include "textmode.h"
#include "bool.h"
#include "misc.h"

bool findRSDP() {
	
	char correctSignature[8] = {'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '};
	
	/* Check region between 0xE0000 and 0xFFFFF. */
	void *pointer;
	for(pointer = (void *)0xE0000; (unsigned int)pointer < 0xFFFFF; pointer += 16) {
		
		/* Dereference and search for pattern. */
		struct RSDPDescriptor *possibleRSDP = pointer;
		
		unsigned char found = 1;
		
		for(unsigned int i = 0; i < 8; i++) {
			if(possibleRSDP->signature[i] != correctSignature[i]) {
				found = 0;
			}
		}

		if(found) {
			
			/* Verify checksum by adding up bytes of RSDP. */
			unsigned char *tmpBuffer;
			unsigned char total = 0;
			tmpBuffer = (unsigned char *)possibleRSDP;
			
			for(unsigned int i = 0; i < sizeof(struct RSDPDescriptor); i++) {
				total += tmpBuffer[i];
			}
			
			if(total == 0) {
				
				RSDP = possibleRSDP;
				return true;
				
			} else {
				
				ERROR("RSDP checksum not zero\n");
				return false;
			
			}
		}
	}
	
	ERROR("couldn't find RSDP\n");
	return false;

}

bool checksum(struct SDTHeader *header) {
	
	uint8 total = 0;
	
	/* Some dirty pointer arithmetic to sum up table. */
	for(unsigned i = 0; i < header->length; i++) {
		total += ((uint8 *) header)[i];
	}

	return total;
	
}

bool initRSDT() {
	
	/* Verify checksum */
	uint8 sum;
	RSDT = (struct RSDT *)RSDP->RSDTAddress;
	
	if((sum = checksum(&RSDT->header)) != 0) {

		ERROR("RSDT checksum not zero\n");
		return false;

	}

	return true;
	
}

/* Find an ACPI table based on the signature. 2*/
struct SDTHeader *findTable(char signature[4]) {

	/* Find number of entries in RSDT */
	unsigned int numEntries = (RSDT->header.length - sizeof(RSDT->header)) / 4;
	
	/* Address of first header in RSDT */
	unsigned int *firstAddress = (unsigned int *)&(RSDT->otherTables);
	
	/* Loop through entries, look for matching signature */
	for(unsigned int i = 0; i < numEntries; i++) {
		struct SDTHeader *header = (struct SDTHeader *)*firstAddress;
		
		bool match = true;

		for(unsigned int i = 0; i < 4; i++) {
			if(header->signature[i] != signature[i]) {
				match = false;	
			}
		}
		
		if(match) {
			return header;	
		}
		
		firstAddress++;
	}
	
	/* There was no match, return null. */
	return 0;

}
