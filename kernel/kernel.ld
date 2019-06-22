OUTPUT_FORMAT("binary")
ENTRY(early_main)
OUTPUT(kernel.bin)
SECTIONS
{
    .text 0xffff800000100000 : ALIGN(0x200) {
        TEXT_START = .;
        *(.early_text)
        *(.text)
        TEXT_END = .;
    } = 0xcccccccc
    .rodata BLOCK(0x200)  : ALIGN(0x200){
        RODATA_START = .;
        *(.rodata)
        RODATA_END = .;
    } = 0xcccccccc
    .data BLOCK(0x200)  : ALIGN(0x200) {
        DATA_START = .;
        *(.data)
        DATA_END = .;
    } = 0xcccccccc

    .bss BLOCK(0x200) : ALIGN(0x200) {
        BSS_START = .;
        *(.bss)
        *(COMMON)
        BSS_END = .;
    } = 0xcccccccc
    /DISCARD/ : {
        *(.eh_frame)
    }
}