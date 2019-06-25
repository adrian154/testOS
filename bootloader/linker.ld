OUTPUT_FORMAT("binary")
ENTRY(cmain)

phys = 0x000010000;

SECTIONS
{
	.text : AT(phys)
	{
		code = .;
		*(.text)
		*(.rodata)
		. = ALIGN(4K);		
	}
	
	.data : AT(phys + (data - code))
	{
		data = .;
		*(.data)
		. = ALIGN(4K);
	}
	
	.bss : AT(phys + (bss - code))
	{
		bss = .;
		*(.bss)
		. = ALIGN(4K);
	}
	
	end = .;
}
