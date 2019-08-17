#pragma once
#include<stdint.h>
#include<memory/address.h>

#include<compiler.h>
#include<intrins.h>

namespace x86_64
{
//https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf
//128 page.
#pragma pack(push, 1)
struct PageTableEntry
{
    bool Present : 1;
    bool RW : 1;
    bool US : 1;
    bool PWT : 1;
    bool PCD : 1;
    bool A : 1;
    bool D : 1;
    bool PAT : 1;
    bool Global : 1;
    uint8_t avail : 3;
    uint32_t base : 28;
    uint16_t reserved2 :12;
    uint16_t avail2 : 11;
    bool EXB : 1;
};
struct PageDirectoryEntry
{
    bool Present : 1;
    bool RW : 1;
    bool US : 1;
    bool PWT : 1;
    bool PCD : 1;
    bool A : 1;
    uint8_t reserved1 : 3;
    uint8_t avail1 : 3;
    uint32_t base : 28;
    uint16_t reserved2 :12;
    uint16_t avail2 : 11;
    bool EXB : 1;
};
struct PageDirectoryEntry2MB
{
    bool Present : 1;
    bool RW : 1;
    bool US : 1;
    bool PWT : 1;
    bool PCD : 1;
    bool A : 1;
    bool D : 1;
    bool PS : 1;
    bool Global : 1;
    uint8_t avail : 3;
    bool PAT : 1;
    uint8_t reserved1 : 8;
    uint32_t base : 19;
    uint16_t reserved2 :12;
    uint16_t avail2 : 11;
    bool EXB : 1;

    void pointTo(uintptr_t ptr)
    {
        base = ptr >> 21;   
    }

};
struct PageDirectoryPointerEntry
{
    bool Present : 1;
    bool RW : 1;
    bool US : 1;
    bool PWT : 1;
    bool PCD : 1;
    bool A : 1;
    uint8_t reserved1 : 3;//P bit
    uint8_t avail1 : 3;
    uint32_t base : 28;
    uint16_t reserved2 :12;
    uint16_t avail2 : 11;
    bool EXB : 1;

    void pointTo(uintptr_t b){
        base = b >> 12;
    }
};
struct PageMapLevel4
{
    bool Present : 1;
    bool RW : 1;
    bool US : 1;
    bool PWT : 1;
    bool PCD : 1;
    bool A : 1;
    uint8_t reserved1 : 3;
    uint8_t avail1 : 3;
    uint32_t base : 28;
    uint16_t reserved2 :12;
    uint16_t avail2 : 11;
    bool EXB : 1;

    void pointTo(uintptr_t b){
        base = b >> 12;
    }
};

namespace Page
{
enum Type{
    P = 1,
    //read/write
    RW = 1 << 1,
    //user mode
    US = 1 << 2,
    //page write through
    PWT = 1 << 3,
    //page cache disable
    PCD = 1 << 4,
    //accessed bit
    A = 1 << 5,
    //dirty bit
    D = 1 << 6,
    //Page Size
    PS = 1 << 7,
};
} // namespace Page

force_inline void load_PML4(PageMapLevel4 * ptr)
{
    write_cr3(ker_addr_t{ptr}.to_phys());
}

#pragma pack(pop)

} // namespace x86_64