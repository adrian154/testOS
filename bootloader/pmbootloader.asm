; pmbootloader.asm: Protected mode "bootloader"; sets up environment for OS.

; start environment: ??? 
; loaded in protected mode at 0x8400
BITS 32
ORG 0x8400

; entry point
start:           
	
	; Test - print a "T"
	mov word [0xB8000], 0x1554

	cli
	hlt

;------------------------------------------------------------------------------;
    
times 512-($-$$) db 0