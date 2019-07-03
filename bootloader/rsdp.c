#include "rsdp.h"
#include "textmode.h"

struct RSDPDescriptor *findRSDP() {
	
	char correctSignature[8] = {'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '};
	
	/* Check region between 0xE0000 and 0xFFFFF. */
	void *pointer;
	for(pointer = (void *)0xE0000; (unsigned int)pointer < 0xFFFFF; pointer += 16) {
		struct RSDPDescriptor *RSDP = pointer;
		
		unsigned char found = 1;
		
		for(unsigned int i = 0; i < 8; i++) {
			if(RSDP->signature[i] != correctSignature[i]) {
				found = 0;
			}
		}

		if(found) {
			return pointer;
		}
	}
	
	return 0;
}
