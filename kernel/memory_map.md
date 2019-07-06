Physical Map
=============
| Name                    | Origin | Length | End    |
| :---------------------- | ------ | ------ | ------ |
| disk_number             | 0x500  | 4      | 0x504  |
| vbe_block               | 0x504  | 0x100  | 0x604  |
| system_address_map_size | 0x60C  | 4      | 0x610  |
| system_address_map      | 0x610  | 0x11f0 | 0x1800 |
| stage2_address          | 0x1800 | 0x800  | 0x2000 |
| disk_buffer_address     | 0x2000 | 0x1000 | 0x3000 |
| PageTableAddress        | 0x3000 | 0x3000 | 0x6000 |
| EarlyMemory             | 0x6000 | 0x1000 | 0x7000 |
| BootSectorCode          | 0x7c00 | 0x200  | 0x7f00 |