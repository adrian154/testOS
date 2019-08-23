; pmbootloader.asm: Protected mode "bootloader"; sets up environment for OS.
; then jumps to OS

; start environment: ??? 
; loaded in protected mode at 1M
BITS 32

; entry point, also declare extern that is the C part of our kernel
GLOBAL start
EXTERN cmain

start:           

	; set up stack
	mov ebp, stack_bottom
	mov esp, stack_top

	; call kernel
	call cmain
	
	; halt
	jmp hang

;------------------------------------------------------------------------------;
; hang: hang the system	

hang:
	cli
	hlt
	jmp hang
    
;------------------------------------------------------------------------------;
; stack: reserve 4K for system stack

ALIGN 16
stack_bottom:
	resb 4096
stack_top:
	    
times 4608-($-$$) db 0
