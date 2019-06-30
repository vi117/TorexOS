OUTPUT_FORMAT("binary")
ENTRY(early_main)
OUTPUT(kernel.bin)
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
    .data BLOCK(0x200)  : ALIGN(0x200) {
        DATA_START = .;
        *(.data .data.*)
        DATA_END = .;
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
    __ctor_list = .;
	.ctors : { *(.ctors) }
	__ctor_end = .;
	__dtor_list = .;
	.dtors : { *(.dtors) }
	__dtor_end = .;

    .bss BLOCK(0x200) : ALIGN(0x200) {
        BSS_START = .;
        *(.bss)
        *(COMMON)
        BSS_END = .;
    } = 0xcccccccc
    KERNEL_END = .;
    /DISCARD/ : {
        *(.comment .note.* .eh_frame)
    }
}