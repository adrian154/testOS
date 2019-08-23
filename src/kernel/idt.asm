GLOBAL loadIDT
EXTERN IDTPointer

loadIDT:
	lidt [IDTPointer]
	ret
