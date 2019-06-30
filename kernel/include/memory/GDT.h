#include <stdint.h>
#ifndef _GDT_H_
#define _GDT_H_

namespace GDT
{
enum class Type
{
    Code = 0x0A,
    Data = 0x02,
    TSS = 0x09,
};
#pragma pack(push,1)
struct GDTR
{
    uint16_t Limit;
    uint64_t BaseAddress;
    uint16_t padding;
};

struct Entry8
{
    uint16_t LowerLimit;
    uint16_t LowerBaseAddress;
    uint8_t MiddleBaseAddress;
    
    Type Type : 4;
    //Descriptor type. This bit should be set for code or data segments and should be cleared for system segments
    bool SystemSegment : 1;
    //Descriptor Privilege Level
    uint8_t DPL : 2;
    //이 세그먼트가 유효
    bool Present : 1;
    uint8_t HighLimit : 4;
    //
    bool Available : 1;
    //if this is code segment on x86_64, it must be true.
    bool L : 1;
    bool DefaultOperationSize : 1;
    //이게 true 면 limit*4KB 이다.
    bool Granularity : 1;
    uint8_t HighBaseAddress;

    void setZero(){
        *reinterpret_cast<uint64_t * const>(this) = 0;
    }
    void setDefault(bool isSystemSegment){
        setZero();
        if(isSystemSegment){
            SystemSegment = true;
            L = true;
        }
        Present = true;
        Granularity = true;
    }

    void setAddress(uint32_t BaseAddress)
    {
        LowerBaseAddress = BaseAddress & 0xFFFF;
        MiddleBaseAddress = (BaseAddress >> 16) & 0xFF;
        HighBaseAddress = (BaseAddress >> 24) & 0xFF;
    }
    void setLimit(uint32_t Limit)
    {
        LowerLimit = Limit & 0xFFFF;
        HighLimit = (Limit >> 16) & 0x0F;
    }
};

struct Entry16 : public Entry8
{
    uint32_t ExtendBaseAddress;
    uint32_t reserved;
    void setZero()
    {
        Entry8::setZero();
        ExtendBaseAddress = 0;
        reserved = 0;
    }

    void setAddress(uint64_t BaseAddress)
    {
        Entry8::setAddress((uint32_t)(BaseAddress & 0x00000000FFFFFFFF));
        ExtendBaseAddress = BaseAddress >> 32;
    }
};

#pragma pack(pop)

} // namespace GDT
#endif