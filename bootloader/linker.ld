OUTPUT_FORMAT("binary")
ENTRY(start)

phys = 0x00100000;

SECTIONS
{

	. = phys;

	.text : {
		*(.text)
	}
	
	.rodata : {
		*(.rodata)
	}
	
	.data : {
		*(.data)
	}
	
	.bss : {
		*(.bss)
	}
	
	end = .;
}
