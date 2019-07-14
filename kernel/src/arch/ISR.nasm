bits 64

DEFAULT REL

%macro KSAVECONTEXT 0       
    push rbp
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    mov ax, ds     
    push rax    
    mov ax, es
    push rax
    push fs
    push gs
%endmacro

%macro KLOADCONTEXT 0        
    pop gs
    pop fs
    pop rax
    mov es, ax      
    pop rax         
    mov ds, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp
%endmacro

section .text.interrupt

%macro interrupt_handler_head 1-2 ISRCommon
extern %2
interrupt_handler_%1:
    KSAVECONTEXT
    mov rdi, %1
    sub rsp, 8
    call %2
    KLOADCONTEXT
    iretq

%endmacro

interrupt_handler_head 0 ;, ISRDivideError
interrupt_handler_head 1 ;, ISRDebug
interrupt_handler_head 2 ;, ISRNMI          
interrupt_handler_head 3 ;, ISRBreakPoint
interrupt_handler_head 4 ;, ISROverflow
interrupt_handler_head 5 ;, ISRBoundRangeExceeded
interrupt_handler_head 6 ;, ISRInvalidOpcode
interrupt_handler_head 7 ;, ISRDeviceNotAvailable
interrupt_handler_head 8 ;, ISRDoubleFault
interrupt_handler_head 9 ;, ISRCoprocessorSegmentOverrun
interrupt_handler_head 10;, ISRInvalidTSS
interrupt_handler_head 11;, ISRSegmentNotPresent
interrupt_handler_head 12;, ISRStackSegmentFault
interrupt_handler_head 13;, ISRGeneralProtection
interrupt_handler_head 14;, ISRPageFault
;reserved isr
interrupt_handler_head 15;, ISR15
interrupt_handler_head 16;, ISRFPUError
interrupt_handler_head 17;, ISRAlignmentCheck
interrupt_handler_head 18;, ISRMachineCheck
interrupt_handler_head 19;, ISRSIMDError
interrupt_handler_head 20;, ISRVirtualization
%assign i 21
%rep 9
interrupt_handler_head i;, ISRETCException
%assign i i+1
%endrep
interrupt_handler_head 30;, ISRSecurity
interrupt_handler_head 31;, ISRETCException
%assign i 32
%rep 97
interrupt_handler_head i;, ISRETCException
%assign i i+1
%endrep

section .rodata.interrupt
global interrupt_handler_vector
interrupt_handler_vector:
%assign i 0
%rep 128
dq interrupt_handler_%[i]
%assign i i+1
%endrep