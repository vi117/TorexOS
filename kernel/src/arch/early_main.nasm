bits 64
;org 0xFFFF800000000000+0x100000
GLOBAL early_main
;x86_64
section .early_text
early_main:
    nop
    nop
    nop
    nop
    extern STACK_END
    mov rsp, STACK_END
    xor rbp, rbp
    extern main
    call main

