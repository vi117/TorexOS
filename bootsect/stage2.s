%include "setting.inc"

org stage2_address
bits 16

section .text
Stage2Entry:
    mov si, LoadStage2String
    call print

    mov byte [DiskNum], dl

    call A20Test
    cmp ax, 1
    je A20GateSuccess

    ;activate using bios service.
    mov ax, 0x2403
    int 0x15
    jc .A20BiosServiceNotSupported
    cmp ah, 0
    jnz .A20BiosServiceNotSupported

    mov ax, 0x2402
    int 0x15
    jc .A20BiosServiceNotSupported ;couldn't get status
    cmp ah, 0
    jnz .A20BiosServiceNotSupported ; could't get status

    cmp al,1
    jz .A20ActivatedTest ;A20Gate is already activated.

    mov ax, 0x2401
    int 0x15
    jc .A20BiosServiceNotSupported ;couldn't activate the gate
    cmp ah, 0
    jnz .A20BiosServiceNotSupported ;couldn't activate the gate

    .A20ActivatedTest:
    call A20Test
    cmp ax, 1
    je A20GateSuccess

    .A20BiosServiceNotSupported:
    ;activate using keyboard controller
    call A20KeyboardEnable

    call A20Test
    cmp ax, 1
    je A20GateSuccess

    ;activate using system port
    in al, 0x92
    or al, 0x02
    and al, 0xFE
    out 0x92, al

    call A20Test
    cmp ax, 1
    je A20GateSuccess

    mov si, A20GateActivateFailedStr
    call print

    jmp HaltLoop

A20GateSuccess:
    mov si, A20GateOnStr
    call print
EnterProtectedModeTemporarily:
    cli
    push ds
    push es

    lgdt [GDTInfo]

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp TemporalProtectMode
TemporalProtectMode:
    mov bx, 0x10 ;(GDT_begin.data32 - GDT_begin)
    mov ds, bx
    mov es, bx

    and al, 0xfe
    mov cr0, eax
    jmp UnrealMode
UnrealMode:
    pop es
    pop ds
    sti

    mov si, EnterUnrealModeStr
    call print

SetTargetOperatingMode:
    ; Some BIOSs assume the processor will only operate in Legacy Mode. We change the Target
    ; Operating Mode to "Long Mode Target Only", so the firmware expects each CPU to enter Long Mode
    ; once and then stay in it. This allows the firmware to enable mode-specifc optimizations.
    ; We save the flags, because CF is set if the callback is not supported (in which case, this is
    ; a N)
    pushf
    mov ax, 0xec00
    mov bl, 0x2
    int 0x15
    popf
    
QuerySystemAddressMap:
    ;set buffer to 0
    xor eax, eax
    mov ecx, system_address_map_avil_length / 4
    mov di, system_address_map
    cld
    rep stosd

    xor ebx, ebx ;first call ebx must be 0
    mov di, system_address_map

    .lp:
        mov eax, 0xE820 ; Function code
        mov edx, 0x534D4150 ; Signiture 'SMAP'
        mov ecx, 24 ;ask for 24 bytes
        mov dword [di + 20], 1   ; force a valid ACPI 3.X entry
        int 0x15
        jc .error
        cmp eax, 0x534D4150
        jne .error

        test byte [di + 20], 1  ;check Extended Attributes bit. if it is 0, ignore current entry.
        jz .skip_entry

        mov ecx, dword [di + 8] ;check length is 0
        or ecx, dword [di + 12]
        jz .skip_entry

        test ebx, ebx
        je .query_end
        add di, 24
        cmp di, system_address_map_end
        jmp .lp
        .skip_entry:
        test ebx, ebx
        je .query_end
        cmp di, system_address_map_end
        jb .lp
    .error:
        mov si, QuerySystemAddressMapErrorStr
        call print
        jmp HaltLoop
    .query_end:
        mov si, QuerySystemAddressMapSuccessStr
        call print
        sub edi, system_address_map - 24
        mov dword [system_address_map_size], edi

    
CheckMemoryEnough:
    mov edi, system_address_map
    mov ecx, [system_address_map_size]
    add ecx, edi
    .lp:
        mov eax, [edi + 16]
        cmp eax, 1
        jne .nextlp
        mov eax, [edi]
        add eax, [edi + 8]
        mov ebx, [edi + 4]
        adc ebx, [edi + 12]
        test ebx, ebx
        jnz .enoughMemory
        cmp eax, 0x03fe0000 ; I think 0x03fe0000 is enough
        jae .enoughMemory
        .nextlp:
        add edi, 24
        cmp edi, ecx
        jb .lp
    .failed:
    mov si, MemoryNotEnoughStr
    call print
    jmp HaltLoop
    .enoughMemory:
    mov si, MemoryEnoughStr
    call print
    
LoadKenerl:
    mov ebx, (KernelSize / 512)         ;total count
    mov edi, KernelLoadedAddress        ;address
    mov eax, ((512+stage2_size) / 512)  ;LBA address
    mov dword [DAPACK.addr], eax
    mov word [DAPACK.offset], disk_buffer_address
    .loop:
        mov eax, (disk_buffer_size / 512)
        cmp ebx, eax
        cmovb eax, ebx

        mov word [DAPACK.count], ax
        mov si, DAPACK
        mov ah, 0x42
        mov dl, byte [DiskNum]
        int 0x13
        jc .LoadKernelError
        
        mov si, disk_buffer_address
        mov edx, dword [DAPACK.addr]

        xor ecx, ecx
        mov ax, word [DAPACK.count]    ;LBA_count
        mov cx, ax
        shl ecx, 9                     ;Address_count = LBA_count*512
        
        .copylp:
            mov al,byte [esi]
            mov byte [edi], al
            inc edi
            inc esi
        loop .copylp    ;edi plus

        mov ax, word [DAPACK.count]    ;LBA_count

        add edx, eax            ;   lba_address += count
        sub ebx, eax            ;   total_count -= count
        mov dword [DAPACK.addr], edx

        cmp ebx, 0
        je .loadingSuccess
    .LoadKernelError:
        mov si, LoadKernelErrorStr
        call print
        jmp HaltLoop
    .loadingSuccess:
        mov si, LoadingSuccessStr
        call print

EnterProtectedMode:
    jmp ProtectedModeEntry

HaltLoop:
    sti
    .spin:
    hlt
    jmp .spin
%if 0
printMemory:
    push cx
    push bx
    mov bx, 0
    mov cx, 64
    .plp:
        mov bx, word [si]
        add si, 2
        call printHex16
        loop .plp
    pop bx
    pop cx
    ret

printDapack:
    pusha
    mov al, 13
    call printChar

    mov bx, [DAPACK.addr + 2]
    call printHex16

    mov bx, [DAPACK.addr]
    call printHex16

    mov al, '#'
    call printChar

    mov bx, [DAPACK.count]
    call printHex16

    mov al, ' '
    call printChar

    mov bx, [DAPACK.seg]
    call printHex16

    mov al, ':'
    call printChar

    mov bx, [DAPACK.offset]
    call printHex16
    popa
    ret
%endif

;when success return 1
;otherwise failed
A20Test:
	pushf
    push si
    push di
	push es
	push ds
	xor ax, ax ;ax is 0x0000
	mov ds, ax
	mov si, 0x0500
	
	not ax  ;ax is 0xffff
	mov es, ax
	mov di, 0x0510  ; 0x0510 + 0xffff0 = 0x10500
	
    mov ax , word [ds:si]
    push ax
    mov ax , word [es:di]
    push ax

	mov word [ds:si], 0x4321
	mov word [es:di], 0x1234
	mov ax, 0x4321
	cmp ax, word [ds:si]

    pop ax
    mov word [ds:si], ax
    pop ax
    mov word [es:di], ax
    mov ax, 1
	je .A20TestSuccess
    mov ax, 0
	.A20TestSuccess:
	pop ds
	pop es
    pop di
    pop si
	popf
	ret

KeyboardWaitInput:
	in al, 0x64
	test al, 2
	jnz KeyboardWaitInput
	ret

KeyboardWaitOutput:
	in		al,	0x64
	test	al,	1
	jz		KeyboardWaitOutput
	ret

A20KeyboardEnable:
	cli
	pusha
		call    KeyboardWaitInput   ; 버퍼 빌때까지 대기
		mov		al, 0xAD	; 0xAD = 키보드 Disable
		out		0x64, al	; 명령어 전송
        call    KeyboardWaitInput	; 버퍼가 빌때(전송이 완료)까지 대기
        
		mov		al, 0xD0	; 0xD0 = 컨트롤러의 데이터 읽음
		out		0x64, al
		call	KeyboardWaitInput	; 컨트롤러 값이 버퍼에 기록될때 까지 대기
		
		in      al, 0x60    ; 버퍼에 기록되면 데이터를 읽어온다
		push    ax			; 데이터를 스택에 저장한다
		call    KeyboardWaitInput

		mov		al, 0xD1    ; 0xD1 = 출력버퍼의 내용을 컨트롤러가 읽음
        out		0x64, al    ; 하지만 출력버퍼가 비어있으므로                          
        call	KeyboardWaitInput   ; 출력 버퍼가 찰 때까지 기다릴 것이다

		pop		ax
		or		al, 2		; 2진수(10) = A20 Gate Enable
		out		0x60, al	; 출력 버퍼에 전송
		call	KeyboardWaitInput   ; 따라서 컨트롤러가 값을 읽어 갈 것이다

        mov     al, 0xAE    ; Keyboard Enable
        out     0x64, al
		call    KeyboardWaitInput
	popa
	sti
	ret

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
;bx = number to print
printHex16:
    push ax
    push cx

    mov cx, 4
    .lp:
    mov al, bh
    shr al, 4
    cmp al, 0xA
    jb .below_0xA
    add al, 'A' - 0xA - '0'
    .below_0xA:
    add al, '0'
    call printChar
    shl bx, 4
    loop .lp

    pop cx
    pop ax
    ret

;In : eax = number to print
printHex:
    push ebx
    mov ebx, eax
    shr ebx, 16
    call printHex16
    mov bx, ax
    call printHex16
    pop ebx
    ret

printLine:
    push ax
    mov al, 13
    call printChar
    mov al, 10
    call printChar
    pop ax
    ret

; arg : al = charactor to print
printChar:
    pusha
    mov bx, 7
    mov ah, 0xE
    int 0x10
    popa
    ret
DiskNum:
    db 0
LoadStage2String:
db "Stage2 loaded.",13,10,0
A20GateOnStr:
db "A20Gate activate.",13,10,0
A20GateActivateFailedStr:
db "A20Gate activate failed.",13,10,0
EnterUnrealModeStr:
db "Enter unrealmode.",13,10,0
QuerySystemAddressMapErrorStr:
db "Failed to query system address map.",13,10,0
QuerySystemAddressMapSuccessStr:
db "Success to query system address map.",13,10,0
MemoryNotEnoughStr:
db "Memory not enough to load kernel. (need 64MB)",13,10,0
MemoryEnoughStr:
db "Memory enuogh to load kernel.",13,10,0
LoadKernelErrorStr:
db "Kernel loading failed.",13,10,0
LoadingSuccessStr:
db "Kernel loading success.",13,10,0
;times ($ - $$) - ((($ - $$)/2)*2) db 0x00
DAPACK:
    db 0x10 ;   packet size
    db 0    ;   always zero
    .count: dw 0 ; int 13 resets this to # of blocks actually read/written
    .offset:   dw 0 ; memory buffer destination address (0:7c00)
    .seg:   dw 0 ; in memory page zero
    .addr:  dq 0 ; put the lba to read in this spot
align 8, db 0
GDTInfo:
   dw GDT_end - GDT_begin - 1   ;last byte in table
   dd GDT_begin                 ;start of table
GDT_begin:         
    dd 0,0        ; entry 0 is always unused
    .code64:
    dw 0xffff
    dw 0x0000 
    db 0
    db 0x9a;10011010b
    db 0xaf;11001111b
    db 0
    .data64:
    dw 0xffff
    dw 0x0000 
    db 0
    db 0x92;10010010b
    db 0xaf;11001111b
    db 0
    .code32:
    dw 0xffff
    dw 0x0000 
    db 0
    db 0x9a;10011010b
    db 0xcf;11001111b
    db 0
    .data32:
    dw 0xffff
    dw 0x0000 
    db 0
    db 0x92;10010010b
    db 0xcf;11001111b
    db 0
GDT_end:

ProtectedModeEntry:
    cli
    lgdt [GDTInfo]

    mov eax, 0x4000003B	;PG=0, CD=1, NW=0, AM=0, WP=0, NE=1, ET=1, TS=1, EM=0, MP=1, PE=1
    mov cr0, eax		;Protected mode 전환
    jmp dword 0x18:ProtectedMode ;(GDT_begin.code32 - GDT_begin):ProtectedModeEntry
EnterProtectedModeStr:
db "Enter protected mode.",0
LongModeSupportStr:
db "Check to support long mode. Success.",0
LongModeNotSupportStr:
db "This computer don't support long mode.",0
MakePageTableStr:
db "Setting Pagetable.",0
bits 32
ProtectedMode:
    mov ax, 0x20;GDT_begin.data32 - GDT_begin
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x10000-4
    mov ebp, 0x10000-4
    push EnterProtectedModeStr
    push 15
    push 0
    call printMessage

CheckLongModeSupport:
    mov eax, 0x80000001
	cpuid
	and edx, 0x20000000	;29번 비트
    cmp edx, 0
    jne .LongModeSupport
        push LongModeNotSupportStr
        push 16
        push 0
        call printMessage
        jmp FailedLoop
    .LongModeSupport:
    push LongModeSupportStr
    push 16
    push 0
    call printMessage
    
MakePageTable:
    push PageTableAddress
    push 0
    push 0x1000*3   ;4kb * 3
    call memset

    mov eax, (PageTableAddress+0x1000)|(1|(1<<1))
    ;baseaddress=PageTableAddress+0x1000, R/W, P bit set.
    mov [PageTableAddress], eax
    mov [PageTableAddress+ 256*8], eax

    mov dword [PageTableAddress + 0x1000], (PageTableAddress+0x1000*2)|(0x1|0x2)

    mov edi, PageTableAddress+0x1000*2
    mov esi, 0
    .loop:
        mov eax, esi
        or eax, (1|(1<<1)|(1<<7))   ;PageSize, R/W, P bit set.
        mov [edi], eax
        add edi, 8
        add esi, 0x00100000 ;2MB
        cmp edi, PageTableAddress+0x1000*3
        jb .loop
    
    push MakePageTableStr
    push 17
    push 0
    call printMessage

    jmp SwitchToLongMode

FailedLoop:
    cli
    .spin:
    hlt
    jmp .spin

MessageColor: db 0x0B
;message
;y
;x
printMessage:
	push ebp
	mov ebp, esp
	pushad
	
	mov eax, dword [ebp + 12]   ;y
	mov esi, 160
	mul esi
	mov edi, eax
	
	mov eax, dword [ebp + 8]    ;x
	shl eax, 1
	add edi, eax
	
	mov esi, dword [ebp + 16]   ;message
	.MessageLoop:
		mov cl, byte [esi]
		cmp cl, 0
		je .MessageLoopEnd
		mov byte [edi + 0xB8000], cl
		inc esi
		inc edi
		mov cl, byte [MessageColor]
		mov byte [edi + 0xB8000], cl
		inc edi
		jmp .MessageLoop
	.MessageLoopEnd:
	popad
	pop ebp
	ret 12
memset:
    push ebp
    mov ebp, esp
    push eax
    push ecx
    push edi

    mov ecx, [ebp + 8] ; size of structure 
    mov al, [ebp + 12] ; fill byte 
    mov edi, [ebp + 16] ; address of structure 
    rep stosb

    pop edi
    pop ecx
    pop edi
    pop ebp
    ret 12

SwitchToLongMode:
    ; Write back cache and add a memory fence. I'm not sure if this is necessary, but better be on the safe side.
    wbinvd
    mfence

    mov eax, cr4
	or eax, 0x20    ;Set PAE bit
	mov cr4, eax
	
	mov eax, PageTableAddress
	mov cr3, eax
	
	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x0100
	wrmsr
	
	mov eax, cr0
	or eax, 0xE0000000
	xor eax, 0x60000000
    ;set NW = 0, CD = 0, PG = 1
	mov cr0, eax
    jmp 0x08:LongModeEntry
bits 64
LongModeEntry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ss, ax

    mov rax, 18   ;y
	mov rsi, 160
	mul rsi
	mov rdi, rax
	
	mov rax, 0    ;x
	shl rax, 1
	add rdi, rax
	
	mov rsi, EnterLongModeStr   ;message
	.MessageLoop:
		mov cl, byte [rsi]
		cmp cl, 0
		je .MessageLoopEnd
		mov byte [rdi + 0xB8000], cl
		inc rsi
		inc rdi
		mov cl, byte [MessageColor]
		mov byte [rdi + 0xB8000], cl
		inc rdi
		jmp .MessageLoop
	.MessageLoopEnd:
    jmp 0xffff800000100000
    cli
    jmp $
EnterLongModeStr:
db "Enter long mode.",0
times stage2_size - ($ - $$) db 0x00