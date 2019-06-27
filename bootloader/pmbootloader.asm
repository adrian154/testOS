; pmbootloader.asm: Protected mode "bootloader"; sets up environment for OS.
; then jumps to OS

; start environment: ??? 
; loaded in protected mode at 0x8400
BITS 32
;ORG 0x8400

; entry point, also declare extern that is the C part of our kernel
GLOBAL start
EXTERN cmain

start:           
	
	; clear screen
	;call clearscreen

	; set up stack
	mov ebp, stack_bottom + 0x8400
	mov esp, stack_top + 0x8400

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
; clearscreen: clear the screen (see VGA text mode)
; takes: none
; returns: none
; changes: eax

clearscreen:

	; loop through all 2000 characters (80*25) and set to black
	xor eax, eax
	
.loop:

	; exit condition
	cmp eax, 4000
	je .done
	
	; clear byte
	mov byte [eax + 0xB8000], 0

	inc eax

	; loop again
	jmp .loop

.done:
	ret
    
;------------------------------------------------------------------------------;
; stack: reserve 4K for system stack

ALIGN 16
stack_bottom:
	resb 4096
stack_top:
	    
times 4608-($-$$) db 0
