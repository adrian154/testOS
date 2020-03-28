global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

isr0:
	cli
	push byte 0
	push byte 0
	jmp isrWrapper

isr1:
	cli
	push byte 0
	push byte 1
	jmp isrWrapper

isr2:
	cli
	push byte 0
	push byte 2
	jmp isrWrapper

isr3:
	cli
	push byte 0
	push byte 3
	jmp isrWrapper

isr4:
	cli
	push byte 0
	push byte 4
	jmp isrWrapper

isr5:
	cli
	push byte 0
	push byte 5
	jmp isrWrapper

isr6:
	cli
	push byte 0
	push byte 6
	jmp isrWrapper

isr7:
	cli
	push byte 0
	push byte 7
	jmp isrWrapper

isr8:
	cli
	push byte 8						; Zero is not pushed because an error code (always zero) is automatically pushed during a double fault.
	jmp isrWrapper

isr9:
	cli
	push byte 0
	push byte 9
	jmp isrWrapper

isr10:
	cli
	push byte 10					; Invalid TSS also pushes error code.
	jmp isrWrapper

isr11:
	cli
	push byte 11					; Segment Not Present also pushes error code.
	jmp isrWrapper

isr12:
	cli
	push byte 12					; Stack-Segment Fault also pushes error code.
	jmp isrWrapper

isr13:
	cli
	push byte 13					; General Protection Fault also pushes error code.
	jmp isrWrapper

isr14:
	cli
	push byte 14					; Page Fault also pushes error code.
	jmp isrWrapper

isr15:
	cli
	push byte 0
	push byte 15
	jmp isrWrapper

isr16:
	cli
	push byte 0
	push byte 16
	jmp isrWrapper

isr17:
	cli
	push byte 17					; Alignment Check also pushes error code.
	jmp isrWrapper

isr18:
	cli
	push byte 0
	push byte 18
	jmp isrWrapper

isr19:
	cli
	push byte 0
	push byte 19
	jmp isrWrapper

isr20:
	cli
	push byte 0
	push byte 20
	jmp isrWrapper

isr21:
	cli
	push byte 0
	push byte 21
	jmp isrWrapper

isr22:
	cli
	push byte 0
	push byte 22
	jmp isrWrapper

isr23:
	cli
	push byte 0
	push byte 23
	jmp isrWrapper

isr24:
	cli
	push byte 0
	push byte 24
	jmp isrWrapper

isr25:
	cli
	push byte 0
	push byte 25
	jmp isrWrapper

isr26:
	cli
	push byte 0
	push byte 26
	jmp isrWrapper

isr27:
	cli
	push byte 0
	push byte 27
	jmp isrWrapper

isr28:
	cli
	push byte 0
	push byte 28
	jmp isrWrapper

isr29:
	cli
	push byte 0
	push byte 29
	jmp isrWrapper

isr30:
	cli
	push byte 30					; Security Exception also pushes error code.
	jmp isrWrapper

isr31:
	cli
	push byte 0
	push byte 31
	jmp isrWrapper
	
extern faultHandler

isrWrapper:
		; CS, EIP, EFLAGS, SS, and ESP are currently on stack

		; PUSHA pushes EAX, ECX, EDX, EBX, ESP (pre-push), EBP, ESI, EDI
		pusha
		
		push ds
		push es
		push fs
		push gs 
		
		; Load data segment descriptor (GDT)
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		
		; Push stack pointer
		mov eax, esp
		push eax
		
		; Indirect call to preserve eip
		mov eax, faultHandler
		call eax
		
		; Restore stack
		pop eax
		
		; Restore segment registers
		pop gs
		pop fs
		pop es
		pop ds
		
		; Pops EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
		popa
		
		; Clean up pushed error code and ISR numbers
		add esp, 8
		
		iret
		
