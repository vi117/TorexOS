OUTPUT_FORMAT("binary")
ENTRY(BootEntry)
STARTUP(early_main.o)
OUTPUT(image.bin)
SECTIONS
{
    .=0xffff800000100000
    .text : ALIGN(0x200) {
        _TEXT_START_ = .;
        *(.text)
        _TEXT_END_ = .;
    } = 0xcccccccc

    .data : ALIGN(0x200) {
        _DATA_START_ = .;
        *(.data)
        _DATA_END_ = .;
    }

    .bss : ALIGN(0x200) {
        _BSS_START_ = .;
        *(.bss)
        _BSS_END_ = .;
    }
}