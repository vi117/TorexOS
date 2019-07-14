OUTPUT_FORMAT(elf64-x86-64)
ENTRY(early_main)
OUTPUT(kernel.elf)
SECTIONS
{
    KERNEL_BEGIN = .;
    .text 0xffff800000100000 : ALIGN(0x200) {
        TEXT_START = .;
        *(.early_text)
        *(.text .text.*)
        TEXT_END = .;
    } = 0xcccccccc
    .rodata BLOCK(0x200)  : ALIGN(0x200){
        RODATA_START = .;
        *(.rodata .rodata.*)
        RODATA_END = .;
    } = 0xcccccccc
    .init_array :{
        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;
    }
    .fini_array :{
        __fini_array_start = .;
        KEEP(*(.fini_array))
        __fini_array_end = .;
    }
    /*__ctor_list = .;
	.ctors : { *(.ctors) }
	__ctor_end = .;
	__dtor_list = .;
	.dtors : { *(.dtors) }
	__dtor_end = .;*/
    .data BLOCK(0x200)  : ALIGN(0x200) {
        DATA_START = .;
        *(.data .data.*)
        DATA_END = .;
    } = 0xcccccccc

    .bss BLOCK(0x200) : ALIGN(0x200) {
        BSS_START = .;
        *(.bss)
        *(COMMON)
        BSS_END = .;
    } = 0xcccccccc
    KERNEL_END = .;
    STACK_START = ALIGN(1M);
    STACK_END = STACK_START + 1M;
    IST_START = STACK_END;
    IST_END = IST_START + 1M;

     /DISCARD/ : {
        *(.plt .plt.* .gnu.* .dynsym .dynstr .dynbss .dynamic .interp .rela.* .got .got.plt .stab .stabstr .comment .note.* .eh_frame .eh_frame_hdr)
    }
}