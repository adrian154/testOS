GLOBAL loadGDT
extern GDTPointer

loadGDT:
	lgdt [GDTPointer]
	
	; Move data segment descriptor into segment registers.
	;mov ax, 0x10
	;mov ds, ax
	;mov es, ax
	;mov fs, ax
	;mov gs, ax
	;mov ss, ax
	
	; Trick CPU into putting 0x08 (code segment) into CS.
	;jmp 0x08:finish
	
;finish:

	; Go back to C.
	ret
