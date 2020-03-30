; Enable paging.
global loadPageDirectory
global enablePaging

; proto: void loadPageDirectory(uint32 */uint32)
loadPageDirectory: 
    
    ; prologue
    push ebp
    mov ebp, esp
    
    ; pop uint32, put into cr3
    mov eax, [esp + 8]
    mov cr3, eax

    ; restore
    mov esp, ebp
    pop ebp
    ret

; proto: void enablePaging()
enablePaging:

    ; set paging bit in cr0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret