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

	; test a20 line; do some setting up.
	mov si, a20set
	call print
	call set_a20
	cmp ax, 0
	je .a20_fail

	; set protected mode
	mov si, pmstart
	call print
	call set_pm

    ; hang system.
	jmp hang
	
.mmap_fail:
	push ax
	mov si, mmap_fail_msg
	pop ax
	mov bx, ax
	call print_hex_byte
	call print
	jmp hang
	
.a20_fail:
	mov si, a20fail
	call print
	jmp hang
	

;------------------------------------------------------------------------------;	
; set_a20: sets a20 line with various methods.
; inputs: none 
; changes: si, ax
; returns: 0 for failure, 1 for success

set_a20:
	
	call test_a20
	cmp ax, 0
	je seta20_bios
	jne .done
	
	mov si, a20_bios_fail
	call print
	
	call test_a20
	cmp ax, 0
	je seta20_kbc              		
	jne .done
	
	mov si, a20_kbc_fail
	call print
	
	call test_a20
	cmp ax, 0
	je seta20_fast
	jne .done
	
	mov si, a20_fast_fail
	call print
	
	jmp .fail
	
.done:
	mov ax, 1
	ret
	
.fail:
	mov ax, 0
	ret
	
a20_bios_fail db "Failed to set A20 via BIOS, trying other method...",13,10,0
a20_kbc_fail db "Failed to set A20 via keyboard controller, trying other method...",13,10,0
a20_fast_fail db "Failed to set A20 via FAST A20. Aborting boot.",13,10,0
	
;------------------------------------------------------------------------------;
; seta20_kbc: sets A20 line with keyboard controller.
; inputs: none
; changes: ax
; returns: nothing

seta20_kbc:
	cli
	
	; send command 0xAD (disable keyboard)
	call a20wait_send
	mov al, 0xAD
	out 0x64, al
	
	; send command 0xD0 (read output port and place into output buffer)
	call a20wait_send
	mov al, 0xD0
	out 0x64, al
	
	; read byte and store on stack
	call a20wait_read
	in al, 0x60
	push ax
	
	; send command 0xD1 (write to output port)
	call a20wait_send
	mov al, 0xD1
	out 0x64, al
	
	; send the byte we read from output port earlier to the output port again.
	call a20wait_send
	pop ax
	or al, 2
	out 0x60, al
	
	; re-enable the keyboard
	call a20wait_send
	mov al, 0xAE
	out 0x64, al
	
	; wait and return
	call a20wait_send
	sti
	ret
	
;------------------------------------------------------------------------------;
; a20wait_send: waits until keyboard controller sends an OK to send bytes.
; inputs: none
; changes: AX
; returns: none

a20wait_send:
	in al, 0x64		; read keyboard controller status
	test al, 2		; bit 2 in status byte = OK to write?
	jnz a20wait_send
	ret
	
;------------------------------------------------------------------------------;
; a20wait_read: waits until keyboard controller sends an OK to read bytes.
; inputs: none
; changes: AX
; returns: none

a20wait_read:
	in al, 0x64		; read status
	test al, 1		; bit 1 = OK to read?
	jnz a20wait_read
	ret
											
;------------------------------------------------------------------------------;
; seta20_fast: sets A20 line with "fast" method
; inputs: none
; changes: none
; returns: nothing

seta20_fast:
	push ax
	
	in al, 0x92
	test al, 2
	jnz .done
	
	or al, 2    
	and al, 0xFE
	out 0x92, al

.done:
	pop ax
	ret	
	
;------------------------------------------------------------------------------;
; seta20_bios: sets A20 line with BIOS method
; inputs: none
; changes: AX
; returns: 0=OK, 1=Not supported, 2=Failed otherwise

seta20_bios:
	mov ax, 0x2403
	int 0x15	
	jb .notsupported
	cmp ah, 0
	jnz .notsupported
	
	mov ax, 0x2402
	int 0x15
	jb .failed
	cmp ah, 0
	jnz .failed
	
	cmp al, 1
	jz .activated
	
	mov ax, 0x2401
	int 0x15
	jb .failed
	cmp ah, 0
	jnz .failed
	
.notsupported:
	mov ax, 1
	ret
	
.failed:
	mov ax, 2
	ret
	
.activated:
	mov ax, 0
	ret
	
;------------------------------------------------------------------------------;
; test_a20: test if A20 line is enabled. 
; inputs:
; changes:
; returns: 0 if not set, 1 if set

test_a20:
	pushf
	push ds
	push es
	push di
	push si
	
	; test by 
	
	; set up segments
	cli
	xor ax, ax
	mov es, ax
	
	not ax
	mov ds, ax
	sti
	
	; set up ds, si
	mov di, 0x0500
	mov si, 0x0510
	
	; push information at locations we'll be testing so it can be restored later
	mov al, byte [es:di]
	push ax
	
	mov al, byte [ds:si]
	push ax
	
	; set 0000:0500 to 00, FFFF:0510 to FF 
	mov byte [es:di], 0x00
	mov byte [ds:si], 0xFF
	
	; check if they are identical
	cmp byte [es:di], 0xFF
	
	; first, restore the locations
	pop ax
	mov byte [ds:si], al	
	pop ax
	mov byte [es:di], al	
	
	; now, compare.
	mov ax, 0
	je .done
	
	mov ax, 1	

.done:
	pop si
	pop di
	pop es
	pop ds
	popf
	
	ret
	
;------------------------------------------------------------------------------;
; do_e820: use BIOS  interrupt 0x15 EAX=E820 to get a reliable memory map
; inputs: nothing 
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
; set_pm: sets up protected mode
; takes:
; changes:
; returns:

set_pm:
	cli
	
	lgdt [gdt_pointer]
	
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	jmp CODE_SEG:pm_boot
	
;------------------------------------------------------------------------------;
; Some various GDT info.
; Not a procedure. DO NOT JUMP HERE.


gdt_start:

; 1st entry is ignored but must be null
gdt_null_entry:
	dq 0x0
	
; entry: Limit=0xFFFFF [x4096 = 0xFFFFFFFF, end of memory]; Base=0x00000000
; access: Present=1, RingLevel=00, Executable=1, Direction=0, ReadWrite=1, Accessed=0
; flags: Granularity=1, Size=1, LimitHigh=1111 (0xF)
gdt_code_entry:			
	dw 0xFFFF			
	dw 0x0				
	db 0x0				
	db 10011010b		
	db 11001111b
	db 0x0
	
; same as code entry, except not executable (1 bit difference in access)
gdt_data_entry:
	dw 0xFFFF			
	dw 0x0				
	db 0x0				
	db 10010010b		
	db 11001111b
	db 0x0
	
; end of gdt
gdt_end:

; structure that points to GDT
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start
	
CODE_SEG equ gdt_code_entry - gdt_start
DATA_SEG equ gdt_data_entry - gdt_start

;------------------------------------------------------------------------------;

; strings
welcome db "stage 2 started.",13,10,0
mmap_fail_msg db "failed to get memory map: ",13,10,0
a20set db "enabling A20 line..",13,10,0
a20fail db "failed to enable A20 line via any method. your hardware may not be supported.",13,10,0.
pmstart db "jumping to protected mode..",13,10,0
			  
;------------------------------------------------------------------------------;
; PROTECTED MODE CODE

bits 32

pm_boot:
	
	; in protected mode, segment registers values hold "descriptors"
	; load segment registers with new full-memory data segment descriptor.
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	sti
	
	mov ebx, 0xB8000
	mov ah, 0x70
	mov al, 'A'
	mov word [ebx], ax

	cli
	hlt
	

;------------------------------------------------------------------------------;

; pad sector with 0s
times 4096-($-$$) db 0