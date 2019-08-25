#include "paging.h"

unsigned int pageDirectory[1024] __attribute__((aligned(4096)));
unsigned int pageTable[256] __attribute__((aligned(4096)));

void setupPaging() {

	/* Set up page directory. */
	/* The directory points to 1024 page tables, which we all set to non-present for the time being. */
	for(unsigned int i = 0; i < 1024; i++) {
		pageDirectory[i] = PDE_KERNEL | PDE_READWRITE;
	}
	
	/* Identity page 1M of memory. */
	for(unsigned int i = 0; i < 1024; i++) {
		pageTable[i] = (i * 0x1000) | PTE_KERNEL | PTE_READWRITE | PTE_PRESENT;
	}
	
	pageDirectory[0] = ((unsigned int)pageTable) | PDE_KERNEL | PDE_READWRITE | PDE_PRESENT;
	
	loadPageDirectory((unsigned int)pageDirectory);
	enablePaging();
	
}
