bits 64
;org 0xFFFF800000000000+0x100000
GLOBAL early_main

section .early_text
early_main:
    nop
    nop
    nop
    nop
    extern STACK_END
    mov rsp, STACK_END
    mov rbp, rsp
    extern main
    jmp main

