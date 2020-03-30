#include "paging.h"

/* 4K aligned page directory, just keep static at compile time */
uint32 pageDirectory[1024] __attribute__((aligned(4096)));

/* TODO MAKE DYNAMIC 4K FRAME ALLOCATOR!!! */
uint32 testPageTable[1024] __attribute((aligned(4096)));

void initPagingTest() {

    /* Clear page directory */
    for(int i = 0; i < 1024; i++) {
        pageDirectory[i] = PDE_NOTPRESENT | PDE_RING0 | PDE_READWRITE;
    }

    /* Set up page table to be identity-mapped */
    for(int i = 0; i < 1024; i++) {
        testPageTable[i] = (i * 0x1000) | PTE_RING0 | PTE_READWRITE | PTE_PRESENT;
    }

    /* Add to page directory */
    pageDirectory[0] = ((uint32)testPageTable) | PDE_PRESENT | PDE_RING0 | PDE_READWRITE;

    loadPageDirectory(pageDirectory);
    enablePaging();

}