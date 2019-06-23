bits 64
;org 0xFFFF800000000000+0x100000
GLOBAL early_main

section .early_text
early_main:
    nop
    nop
    nop
    nop
    mov rsp, 0xffff800000100000+0x100000/2*9-16
    mov rbp, rsp
    extern main
    call main

