stage2_address  equ 0x1000
stage2_size equ 0x1000 ;
stage2_end equ (stage2_address + stage2_size)

disk_buffer_address equ 0x2000
disk_buffer_size equ 0x1000


disk_number     equ 0x500
vbe_block       equ 0x504

system_address_map_size equ 0x610-4
system_address_map  equ system_address_map_size+4
system_address_map_end equ 0x1000
system_address_map_avil_length equ (system_address_map_end - system_address_map)

PageTableAddress equ 0x3000 ;those size is 12kb.

%define KernelSize 1024
%define KernelLoadedAddress 0x100000