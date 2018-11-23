; bootloader.asm: 2nd stage of bootloader

; start environment: CS,ES,GS=0, known stack, registers unknown (DL=disk number)
; loaded from 0x7E00 by bootsector.
BITS 16
ORG 0x7E00

; entry point
start:

	; print message
	mov si, welcome
	call print

    ; hang system.
	jmp hang
	
;------------------------------------------------------------------------------;
	  
; hang: infinitely stop the system.
; inputs: none
; changes: Interrupt flag
; returns: doesn't
hang:
	cli
	hlt
	jmp hang
	
;------------------------------------------------------------------------------;

; print: prints a string
; inputs: SI=Pointer to string
; changes: Direction flag, AX 
; returns: none
print:
	; set up AH for interrupt, direction flag so the string is read correctly
	mov ah, 0x0E
	cld
.loop:
	; Loop through each character of string, print if not null 
	lodsb
	cmp al, 0
	je .done
	int 0x10
	jmp .loop
.done:
	ret
	
;------------------------------------------------------------------------------;
	
; strings
welcome db "stage 2 started.",13,10,0
			  
; pad sector with 0s?
times 512-($-$$) db 0