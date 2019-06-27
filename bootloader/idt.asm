GLOBAL loadIdt
EXTERN idtPointer

loadIdt:
	lidt [idtPointer]
	ret
