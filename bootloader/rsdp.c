#include "rsdp.h"
#include "textmode.h"
#include "bool.h"

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
				
				terminalForeground = BRIGHT_RED;
				printString("findRSDP(): RSDP checksum does not equal zero, got 0x"); printByte(total); putChar('\n');
				return false;
			
			}
		}
	}
	
	printString("findRSDP(): did not find RSDP in 0xE0000 through 0xFFFFF\n");
	return false;
}
