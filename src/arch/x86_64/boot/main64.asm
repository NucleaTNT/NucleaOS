global long_mode_entry
[extern KernelEntry]

section .text
    [bits 64]
    long_mode_entry:
        mov ax, 0
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        ; Print "Hello, World!"
        mov byte [0xb8000],         'H' 
        mov byte [0xb8000 + 2],     'e'
        mov byte [0xb8000 + 4],     'l'
        mov byte [0xb8000 + 6],     'l'
        mov byte [0xb8000 + 8],     'o'
        mov byte [0xb8000 + 10],    ','
        mov byte [0xb8000 + 12],    ' '
        mov byte [0xb8000 + 14],    'W'
        mov byte [0xb8000 + 16],    'o'
        mov byte [0xb8000 + 18],    'r'
        mov byte [0xb8000 + 20],    'l'
        mov byte [0xb8000 + 22],    'd'
        mov byte [0xb8000 + 24],    '!'

        call KernelEntry
    
        hlt