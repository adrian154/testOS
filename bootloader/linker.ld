OUTPUT_FORMAT("binary")
ENTRY(start)

phys = 0x00008400;

SECTIONS
{
	.text : AT(phys)
	{
		code = .;
		*(.text)
		. = ALIGN(4K);		
	}
	
	.rodata : AT(phys + (rodata - code))
	{
		rodata = .;
		*(.rodata*)
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
