; bootsector.asm: bootsector for testOS.

; Start environment: Real mode, physical 0x7C00 (no guarantees about CS/IP)
; DL contains disk number

BITS 16
ORG 0x7C00

; Constants.
SECTORS_TO_LOAD equ 3

; Set CS/IP by doing a far jump.
jmp 0x0000:start

;------------------------------------------------------------------------------;
; Entry point.

start:

	; Set up segments & stack.
	cli
	
	xor ax, ax
	mov ds, ax
	mov es, ax
	
	; Stack grow down from 0x0000:0x7BFF, right below our bootsector.
	mov ss, ax
	mov sp, 0x7BFF
	
	sti
	
	; Set video/text mode.
	call textmode

   	; Show a success message.
   	mov si, welcome
   	call print
   	
   	; Load stage 2 and far jump to it. Save DL on stack first.
   	call load_stage2
   	cmp ah, 0
   	jne .errorloading
	jmp 0x0000:0x7E00
    
	; Hang, just in case.
    jmp hang  
    
; Print message and errorcode, and hang (in case stage 2 can't be loaded).
.errorloading:
	
	push ax
	
	mov si, loaderror
	call print
	
	pop ax
	mov bx, ax
	call print_hex_byte
	
	jmp hang
	
;------------------------------------------------------------------------------; 
; textmode: Enables textmode.

textmode:
	xor ah, ah
	mov al, 0x03
	int 0x10
	ret
	
;------------------------------------------------------------------------------; 
; print: Prints a string.
; inputs: pointer to string in SI
; changes: Direction flag, AX 
; returns: none

print:

	; Set up AH for interrupt, direction flag so the string is read correctly.
	mov ah, 0x0E
	cld

.loop:

	; Loop through each character of string, print if not null.
	lodsb
	cmp al, 0
	je .done

	; Call interrupt and print string.
	int 0x10
	jmp .loop

.done:
	ret 
	
;------------------------------------------------------------------------------;	
; print_hex_byte: Print 1 hex byte in BL.
; inputs:  byte to print in BL
; changes: AX, BX
; returns: none

print_hex_byte:

	; Save BX; put 1st hex digit into BL. Also prepare for printing via int 0x10.
	push bx
	mov ah, 0x0E
	
	; Print 1st digit.
	shr bx, 4
	call print_hex_digit
	
	; Print 2nd digit after restoring BX.
	pop bx 
	and bl, 0xF
	call print_hex_digit
	
	ret

;------------------------------------------------------------------------------;
; print_hex_digit: Print 1 digit of hex in BL
; inputs: digit in BL
; changes: AX
; returns: none

print_hex_digit:

	; Prepare for int 0x10.
	mov ah, 0x0E
	
	; Check if digit is a letter.
	cmp bl, 0xA

	; Print accordingly:
	jl .notletter
	jmp .letter
	
; Not letter: print as number.
.notletter:
	add bl, '0'
	mov al ,bl
	int 0x10
	ret
	
; Letter: print as letter.
.letter:
	sub bl, 0xA
	add bl, 'A'
	mov al, bl
	int 0x10
	ret
	
;------------------------------------------------------------------------------;		
; load_stage2: Loads stage 2 into memory.
; inputs: none
; changes: AX, CX, DX 
; returns: status code (0 = success)

load_stage2:
	
	; Use bios INT 0x13 AH=0x02 to load sectors.
	; AH = function number
	; AL = number of sectors to load
	; CH = cylinder
	; CL = sector
	; DH = head
	; BX = buffer
	mov ah, 0x02
	mov al, SECTORS_TO_LOAD
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov bx, 0x7E00
	int 0x13
	ret
    
;------------------------------------------------------------------------------;
; hang: Infinitely stop the system.
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
	
;------------------------------------------------------------------------------;
; pad with zeroes. leave space for magic number.

times 510-($-$$) db 0
dw 0xAA55	


