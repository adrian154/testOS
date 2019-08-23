#define PDE_PRESENT			0b00000001
#define PDE_READWRITE		0b00000010
#define PDE_USER			0b00000100
#define PDE_KERNEL			0b00000000
#define PDE_WRITETHROUGH	0b00001000
#define	PDE_NOCACHE			0b00010000
#define PDE_IS4M			0b10000000

#define PTE_PRESENT			0b00000001
#define PTE_READWRITE		0b00000010
#define PTE_USER			0b00000100
#define PTE_KERNEL			0b00000000
#define PTE_WRITETHROUGH	0b00001000
#define PTE_NOCACHE			0b00010000
#define PTE_GLOBAL			0b000100000000

extern void loadPageDirectory(unsigned int address);
extern void enablePaging();
extern void setupPaging();