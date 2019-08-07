#pragma once
#include<stdint.h>

namespace x86_64
{

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
};
struct PageDirectoryPointerEntry
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
};
#pragma pack(pop)

} // namespace x86_64