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

	; get memory map
	call do_e820
	cmp ax, 0
	jne .mmap_fail

    ; hang system.
	jmp hang
	
.mmap_fail:
	mov bx, ax
	call print_hex_byte
	mov si, mmap_fail_msg
	call print
	jmp hang
	
;------------------------------------------------------------------------------;
; 
;
;
	
;------------------------------------------------------------------------------;
; do_e820: use BIOS  interrupt 0x15 EAX=E820 to get a reliable memory map
; takes: nothing 
; changes: everything but ESI
; returns: BP=number of entries AL=success?
; 0=OK, 1=Carry was set, 2=Not supported, 3=Bad list

do_e820:
	; set up for interrupt
	mov eax, 0xE820
	mov ebx, 0
	mov di, 0x8E00				; load map at 0x8E00, 1M above bootloader.
	mov ecx, 24					; entries will not be >24bytes large
	mov edx, 0x534D4150			; magic number.
	
	; force to be valid ACPI entry
	; even if BIOS doesn't fill acpi field, it will be 1
	mov [es:di + 20], dword 1
	
	; BP stores entries received, so we zero it
	xor bp, bp
	
	; do interrupt.
	int 0x15
	jc .errorcarryflag
	
	; reload EDX, sometimes trashed. EAX should have 'SMAP' in it to verify BIOS support.
	mov edx, 0x534D4150
	cmp eax, edx
	jne .errornotsupported
	
	; if EBX=0, the last entry has been reached (1 entry long list is worthless)
	cmp ebx, 0
	je .errorbadlist
	
	; check entry
	jmp .check
	
.loop:
	; ECX, EAX trashed by interrupt. set up for interrupt and get next entry
	mov eax, 0xE820
	mov ecx, 24
	mov [es:di + 20], dword 1
	
	int 0x15
	jc .done

	mov edx, 0x534D4150
	
.check:
	; if 0 bytes long, skip
	jcxz .skip
	; check if ACPI. if not, do another routine.
	cmp cl, 20
	jbe .check2
	; check if "ignore" bit is set
	test byte [es:di + 20], 1
	je .skip
	
.check2:
	; check if length 0. if so, skip.
	mov ecx, [es:di + 8]
	or ecx, [es:di + 12]
	jz .skip
	
	; increment entries, advance pointer.
	inc bp
	add di, 24
	
.skip: 
	test ebx, ebx
	jne .loop
	
.done:
	mov ax, 0
	clc
	ret
	
.errorcarryflag:
	mov ax, 1
	ret
	
.errornotsupported:
	mov ax, 2
	ret
	
.errorbadlist:
	mov ax, 3
	ret

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
; print_hex_dword: print 1 hex dword in EBX.
; inputs: byte to print in EBX
; changes: AX, EBX
; returns: none
print_hex_dword:
	push ebx
	mov ah, 0x0E
	
	shr ebx, 16
	call print_hex_word
	
	pop ebx
	and bx, 0xFFFF
	call print_hex_word
	
	ret
	
;------------------------------------------------------------------------------;
; print_hex_word: print 1 hex word in BX.
; inputs: byte to print in BX
; changes: AX, BX
; returns: none
print_hex_word:
	push bx
	mov ah, 0x0E
	
	shr bx, 8
	call print_hex_byte
	
	pop bx
	and bx, 0xFF
	call print_hex_byte
	
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

; strings
welcome db "stage 2 started.",13,10,0
mmap_fail_msg db "failed to get memory map.",13,10,0
			  
; pad sector with 0s?
times 4096-($-$$) db 0