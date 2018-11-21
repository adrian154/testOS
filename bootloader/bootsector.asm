; bootsector.asm: bootsector for testOS.

; start environment - Real mode, physical 0x7C00 (no guarentees about CS/IP)
; DL contains disk number; nothing else!

BITS 16
ORG 0x7C00

; set CS/IP by doing a far jump.
jmp 0x0000:start

; entry point.
start:

	; set up segments & stack.
	cli
	
	xor ax, ax
	mov ds, ax
	mov es, ax
	
	; stack will be at 0x0000:0x7BFF, right below our bootsector
	mov ss, ax
	mov sp, 0x7BFF
	
	sti

   	; show a success message.
   	mov si, welcome
   	call print
   
	; hang.
    jmp hang
    
    
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

    
; hang: infinitely stop the system.
; inputs: none
; changes: Interrupt flag
; returns: doesn't
hang:
	cli
	hlt
	jmp hang
	
; messages displayed to the user.
welcome db "testOS bootsector started.",13,10,0
	
; pad with zeroes. leave space for magic number.
times 510-($-$$) db 0
dw 0xAA55	


