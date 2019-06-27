OUTPUT_FORMAT("binary")
ENTRY(start)

phys = 0x00008400;

SECTIONS
{
	.text BLOCK(4K) : ALIGN(4K) {
		*(.text)
	}
	
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
	
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}
	
	.bss BLOCK(4K) : ALIGN(4K) {
		*(.bss)
	}
	
	end = .;
}
