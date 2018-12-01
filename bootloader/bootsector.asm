; bootsector.asm: bootsector for testOS.

; start environment - Real mode, physical 0x7C00 (no guarentees about CS/IP)
; DL contains disk number; nothing else!

BITS 16
ORG 0x7C00

; constants
SECTORS_TO_LOAD equ 8

; set CS/IP by doing a far jump.
jmp 0x0000:start

;------------------------------------------------------------------------------;

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
   	
   	; load stage 2. if there is an error, print a message.
   	call load_stage2
   	cmp ah, 0
   	jne .errorloading
   
   	; far jump to stage 2.
   	jmp 0x0000:0x7E00
   
	; hang.
    jmp hang  
    
; print message and errorcode, and hang.
.errorloading:
	
	
	push ax
	
	mov si, loaderror
	call print
	
	pop ax
	mov bx, ax
	call print_hex_byte
	
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
	
; print_hex_byte: print 1 hex byte in BL.
; inputs:  byte to print in BL
; changes: AX, BX
; returns: none
print_hex_byte:
	; save bx; put 1st hex digit into BL. also prepare for printing via int 0x10
	push bx
	mov ah, 0x0E
	
	; print 1st digit
	shr bx, 4
	call print_hex_digit
	
	; print 2nd digit after restoring BX
	pop bx 
	and bl, 0xF
	call print_hex_digit
	
	ret

;------------------------------------------------------------------------------;

; print_hex_digit: print 1 digit of hex in BL
; inputs: digit in BL
; changes: AX
; returns: none
print_hex_digit:
	; prepare for int 0x10
	mov ah, 0x0E
	
	; check if digit is a letter
	cmp bl, 0xA
	
	; do according action
	jl .notletter
	jmp .letter
	
; not letter: print a number
.notletter:
	add bl, '0'
	mov al ,bl
	int 0x10
	ret
	
; letter: print a letter
.letter:
	sub bl, 0xA
	add bl, 'A'
	mov al, bl
	int 0x10
	ret
	
;------------------------------------------------------------------------------;	
		
; load_stage2: loads stage 2 into memory
; inputs: none
; changes: AX, CX, DX 
; returns: status code (0 = success)
load_stage2:
	; use bios INT 0x13 AH=0x02 to load sectors.
	mov ah, 0x02		; function number
	mov al, SECTORS_TO_LOAD
	mov ch, 0        	; cylinder
	mov cl, 2 		    ; sector
	mov dh, 0 	        ; head
	mov bx, 0x7E00		; es:bx points to buffer; ES is 0, we want to load to physical 0x7E00 (right after bootsector)
	int 0x13
	ret
    
; hang: infinitely stop the system.
; inputs: none
; changes: Interrupt flag
; returns: doesn't
hang:
	cli
	hlt
	jmp hang
	
;------------------------------------------------------------------------------;
	
; messages displayed to the user.
welcome db "testOS bootsector started.",13,10,0
loaderror db "error loading stage 2.",13,10,0
	
; pad with zeroes. leave space for magic number.
times 510-($-$$) db 0
dw 0xAA55	


