section .multiboot_header
    header_start:
        dd 0xe85250d6                   ; Multiboot2
        dd 0x0                          ; Architecture - ProtMode i386
        dd header_end - header_start    ; Header length
        ; Checksum
        dd 0x100000000 - (0xe85250d6 + 0x0 + header_end - header_start)

        ; End tag
        dw 0x0
        dw 0x0
        dd 0x8
    header_end: