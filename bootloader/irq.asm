global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
	cli
	push byte 32
	jmp irqWrapper

irq1:
	cli
	push byte 33
	jmp irqWrapper

irq2:
	cli
	push byte 34
	jmp irqWrapper

irq3:
	cli
	push byte 35
	jmp irqWrapper

irq4:
	cli
	push byte 36
	jmp irqWrapper

irq5:
	cli
	push byte 37
	jmp irqWrapper

irq6:
	cli
	push byte 38
	jmp irqWrapper

irq7:
	cli
	push byte 39
	jmp irqWrapper

irq8:
	cli
	push byte 40
	jmp irqWrapper

irq9:
	cli
	push byte 41
	jmp irqWrapper

irq10:
	cli
	push byte 42
	jmp irqWrapper

irq11:
	cli
	push byte 43
	jmp irqWrapper

irq12:
	cli
	push byte 44
	jmp irqWrapper

irq13:
	cli
	push byte 45
	jmp irqWrapper

irq14:
	cli
	push byte 46
	jmp irqWrapper

irq15:
	cli
	push byte 47
	jmp irqWrapper
	
extern irqHandler

irqWrapper:
	pusha
	
	push ds
	push es
	push fs
	push gs
	
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov eax, esp
	push eax
	
	mov eax, irqHandler
	call eax
	
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	
	popa
	add esp, 4
	
	sti
	iret
	
	
	
	
	
