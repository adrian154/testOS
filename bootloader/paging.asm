GLOBAL loadPageDirectory
loadPageDirectory:

	; Function prologue: create new stack frame.
	push ebp
	mov ebp, esp
	
	; Pop off 64 bit entry and move into CR3.
	mov eax, [esp+8]
	mov cr3, eax
	
	; Function epilogue: restore old stack frame by restoring EBP. Return.
	mov esp, ebp
	pop ebp
	ret
	
GLOBAL enablePaging
enablePaging:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret