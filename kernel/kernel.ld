OUTPUT_FORMAT("binary")
ENTRY(BootEntry)
STARTUP(stage1.o)
OUTPUT(image.bin)
SECTIONS
{   
    .=0x500
    "disk_number"=.;
    .+=4
    "vbe_block"=.;
    .+=0x100
    "system_address_map_size"=.;
    .+=4;
    "system_address_map"=.;
    .=0x1000
    "system_address_map_end"=.;
    "stage2_address"=.;
    .=0x7c00
    .text . : {
        stage1.o
    }
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