[bits 64]

global _loadGDT
_loadGDT:
    lgdt [rdi]          ; RDI contains first parameter when called from C++
    
    ; Initialize data segments
    mov ax, 0x10        ; Move KernelData selector into AX
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to set code segment
    pop rdi             ; Pop return address into rdi
    mov rax, 0x08       ; KernelCode selector
    push rax            ; Push selector onto stack
    push rdi            ; Push return address onto stack
    retfq               ; Far return to set segment