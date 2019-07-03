bits 64
;org 0xFFFF800000000000+0x100000
GLOBAL early_main

section .early_text
early_main:
    nop
    nop
    nop
    nop
    extern KERNEL_BEGIN, KERNEL_END
    ;why does error occur?
    ;align KERNEL_END to 1MB, add 1MB and set to stack address.
    ;mov rax, KERNEL_END
    ;and rax, 0xfffffffffff00000
    ;add rax, 0x100000 * 3
    ;sub rax, 16
    ;mov rsp, rax
    mov rsp, 0xffff800000100000+0x100000*3-16
    mov rbp, rsp
    extern main
    jmp main

