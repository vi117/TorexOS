%include "setting.inc"

org 0x7c00
bits 16

section .text

BootEntry:
    ;initialize segment register
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov sp, 0x7c00
    ;initialize CS
    jmp 0x00:DiskLoad

DiskLoad:
    mov si, LoadingStartMessage
    call print

    mov ah, 0x41
    mov bx, 0x55aa
    int 0x13
    jc Error

    mov word [DAPACK.count], (stage2_size / 512) ; 127 is max on some BIOSes
    mov word [DAPACK.offset] , stage2_address
    mov word [DAPACK.seg], 0x0
    mov dword [DAPACK.addr], 1
    mov si, DAPACK
    mov ah, 0x42
    int 0x13
    jc Error
    jmp stage2_address
Error:
    mov si, NotSupported
    call print
Loop:
hlt
jmp Loop

;print a string
; arg : si = points at zero-terminated String
print:
    push ax
    pushf
    cld
.loop:
    lodsb
    test al, al
    jz .done
    call printChar
    jmp .loop
.done:
    popf
    pop ax
    ret

printLine:
    mov al, 13
    call printChar
    mov al, 10
    jmp printChar

; arg : al = charactor to print
printChar:
    pusha
    mov bx, 7
    mov ah, 0xE
    int 0x10
    popa
    ret
LoadingStartMessage:
db "Load from disk.",13,10,0
NotSupported:
db "Error!",13,10,0
;times ($ - $$) - ((($ - $$)/2)*2) db 0x00
DAPACK:
    db 0x10 ;   packet size
    db 0    ;   always zero
    .count: dw 0 ; int 13 resets this to # of blocks actually read/written
    .offset:   dw 0 ; memory buffer destination address (0:7c00)
    .seg:   dw 0 ; in memory page zero
    .addr:  dq 0 ; put the lba to read in this spot

;AH 00h = No error
 ;If Carry = 1:
 ;AH 01 = Bad command or parameter
 ;02h = Address mark not found
 ;04h = Sector not found
 ;05h = Reset failed
 ;07h = Drive parameter activity failed
 ;0Ah = Bad sector flag detected
 ;10h = ECC data error
 ;11h = ECC data corrected
 ;20h = Controller failure
 ;40h = Seek failed
 ;80h = Time out occurred
 ;AAh = Drive not ready
 ;BBh = Undocumented controller error
 ;CCh = Controller write fault
 ;E0h = Unrecognized controller error 
 
times 446 - ($ - $$) db 0x00
MBR_area:
times 8 dq 0

db 0x55
db 0xAA