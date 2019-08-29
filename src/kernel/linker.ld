OUTPUT_FORMAT("binary")
ENTRY(start)

. = 0xE0100000;

kernelPhysicalStart = .;

SECTIONS
{

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
	
}

kernelPhysicalEnd = .;
