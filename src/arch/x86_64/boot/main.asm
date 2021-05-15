[global start]
[extern long_mode_entry]

section .text
    [bits 32]
    start:
        mov esp, stack_top

        call check_multiboot
        call check_cpuid
        call check_long_mode

        call set_page_tables
        call enable_paging

        lgdt [gdt64.pointer]
        jmp gdt64.code_seg:long_mode_entry

        hlt

    check_multiboot:
        cmp eax, 0x36d76289 ; Magic
        jne .no_multiboot
        ret

        .no_multiboot:
            call prep_err
            mov byte [0xb8000 + 10],    'M'
            mov byte [0xb8000 + 12],    'U'
            mov byte [0xb8000 + 14],    'L'
            mov byte [0xb8000 + 16],    'T'
            mov byte [0xb8000 + 18],    'I'
            mov byte [0xb8000 + 20],    'B'
            mov byte [0xb8000 + 22],    'O'
            mov byte [0xb8000 + 24],    'O'
            mov byte [0xb8000 + 26],    'T'

            hlt

    check_cpuid:
        pushfd

        pop eax
        mov ecx, eax        ; Preserve original flags

        xor eax, 1 << 21    ; Flip CPU ID bit
        push eax

        popfd
        pushfd

        pop eax
        push ecx            ; Return original flags   
        popfd

        cmp eax, ecx        ; If the bit stays the same then LM not supported
        je .no_cpu_id
        ret

        .no_cpu_id:
            call prep_err
            mov byte [0xb8000 + 10],    'C'
            mov byte [0xb8000 + 12],    'P'
            mov byte [0xb8000 + 14],    'U'
            mov byte [0xb8000 + 16],    'I'
            mov byte [0xb8000 + 18],    'D'

            hlt

    check_long_mode:
        mov eax, 0x80000000
        cpuid
        cmp eax, 0x80000001
        jb .no_long_mode

        mov eax, 0x80000001
        cpuid
        test edx, 1 << 29
        jz .no_long_mode
        ret

        .no_long_mode:
            call prep_err
            mov byte [0xb8000 + 10],    'L'
            mov byte [0xb8000 + 12],    'O'
            mov byte [0xb8000 + 14],    'N'
            mov byte [0xb8000 + 16],    'G'
            mov byte [0xb8000 + 18],    ' '
            mov byte [0xb8000 + 10],    'M'
            mov byte [0xb8000 + 12],    'O'
            mov byte [0xb8000 + 14],    'D'
            mov byte [0xb8000 + 16],    'E'
            
            hlt

    set_page_tables:
        mov eax, page_table_l3
        or eax, 0b11    ; Present, writable
        mov [page_table_l4], eax

        mov eax, page_table_l2
        or eax, 0b11    ; Present, writable
        mov [page_table_l3], eax

        mov ecx, 0

        .loop:
            mov eax, 0x200000   ; 2MB
            mul ecx
            or eax, 0b10000011  ; present, writable, huge page
            mov [page_table_l2 + ecx * 8], eax

            inc ecx
            cmp ecx, 512
            jne .loop
        
            ret
    
    enable_paging:
        ; Pass page table location to cpu
        mov eax, page_table_l4
        mov cr3, eax

        ; Enable PAE
        mov eax, cr4
        or eax, 1 << 5
        mov cr4, eax

        ; Enable long mode
        mov ecx, 0xc0000080
        rdmsr
        or eax, 1 << 8
        wrmsr

        ; Enable paging
        mov eax, cr0
        or eax, 1 << 31
        mov cr0, eax

        ret

    prep_err:
        mov byte [0xb8000],         'E' 
        mov byte [0xb8000 + 2],     'R'
        mov byte [0xb8000 + 4],     'R'
        mov byte [0xb8000 + 6],     ':'
        mov byte [0xb8000 + 8],     ' '
        ret

section .bss
    align 4096
    page_table_l4:
        resb 4096
    page_table_l3:
        resb 4096
    page_table_l2:
        resb 4096

    stack_bottom:
    resb 4096 * 4
    stack_top:

section .rodata
    gdt64:
        dq 0    ; Null entry

        .code_seg: equ $ - gdt64
            dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)

        .pointer:
            dw $ - gdt64 - 1
            dq gdt64