#ifndef __PAGING_H
#define __PAGING_H

#include "types.h"

/* bitmask attributes for page directory entry */
#define PDE_PRESENT         0b00000001
#define PDE_NOTPRESENT      0b00000000
#define PDE_READWRITE       0b00000010
#define PDE_READONLY        0b00000000
#define PDE_USER            0b00000100
#define PDE_RING0           0b00000000
#define PDE_WRITETHROUGH    0b00001000
#define PDE_WRITEBACK       0b00000000
#define PDE_CACHE_DISABLED  0b00010000
#define PDE_CACHE           0b00000000

/* bitmask attributes for page table */
#define PTE_PRESENT         0b00000001
#define PTE_NOTPRESENT      0b00000000
#define PTE_READWRITE       0b00000010
#define PTE_READONLY        0b00000000
#define PTE_USER            0b00000100
#define PTE_RING0           0b00000000
#define PTE_WRITETHROUGH    0b00001000
#define PTE_WRITEBACK       0b00000000
#define PTE_CACHE_DISABLED  0b00010000
#define PTE_CACHE           0b00000000

#endif