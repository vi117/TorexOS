bits 64
;org 0xFFFFC00000000000+0x100000
global early_main

section .early_text
early_main:
    mov rsp, 0xFFFFC00000000000+0x100000/2*8
    mov rbp, rsp
    extern main
    jmp main