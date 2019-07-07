#include <stdint.h>
#ifndef _GDT_H_
#define _GDT_H_

namespace GDT
{

#pragma pack(push, 1)
struct GDTR
{
    uint16_t Limit;
    uint64_t BaseAddress;
    uint16_t padding;
};

enum SegmentType
{
    //Just set to zero. CPU set this to 1 when the segment is accessed.
    AccessBit = 0x1,
    /*ReadWrite bit.
     *Readable bit for code selectors: 
     *   Whether read access for this segment is allowed.
     *   Write never available .
     *Writable bit for data selectors: 
     *   Whether write access for this segment is allowed.
     *   Read always available.
    */
    RWbit = 0x2,
    /*Direction bit/Conforming bit.
     *Direction bit for data selectors: 
        Tells the direction. 0 the segment grows up. 1 the segment grows down.
     *Conforming bit for code selectors:
        If 1 code in this segment can be executed from an equal or lower privilege level.
        If 0 code in this segment can only be executed from the ring set in DPL.
    */
    DCbit = 0x4,
    /*

    */
    Executablebit = 0x8,


    Reserved1           = 0b0000,
    TSS_16bit           = 0b0001,
    LDT                 = 0b0010,
    TSS_16bit_Busy      = 0b0011,
    CallGate_16bit      = 0b0100,
    TaskGate            = 0b0101,
    InterruptGate_16bit = 0b0110,
    TrapGate_16bit      = 0b0111,
    Reserved2           = 0b1000,
    TSS_32bit           = 0b1001,
    Reserved3           = 0b1010,
    TSS_32bit_Busy      = 0b1011,
    CallGate_32bit      = 0b1100,
    Reserved4           = 0b1101,
    InterruptGate_32bit = 0b1110,
    TrapGate_32bit      = 0b1111,
};
struct Entry8
{
    uint16_t LowerLimit;
    uint16_t LowerBaseAddress;
    uint8_t MiddleBaseAddress;

    uint8_t Type : 4;

    //Descriptor type. This bit should be set for code or data segments and should be cleared for system segments
    bool S : 1;
    //Descriptor Privilege Level
    uint8_t DPL : 2;
    //Always it is true.
    bool Present : 1;
    uint8_t HighLimit : 4;
    //Available for use by kernel
    bool Available : 1;
    //64-bit code segment.
    bool L : 1;
    //when L bit is set, it must be false.
    bool DefaultOperationSize : 1;
    //이게 true 면 limit*4KB 이다.
    bool Granularity : 1;
    uint8_t HighBaseAddress;

    void setZero()
    {
        *reinterpret_cast<uint64_t *const>(this) = 0;
    }
    //long mode defualt
    void setLongmodeDefualt(bool isSystemSegment, uint8_t segmentType, uint8_t privilege)
    {
        setZero();
        if(!isSystemSegment)
        {
            setAddress(0);
            setLimit(0xfffff);
        }
        this->Present = true;
        this->Type = segmentType;
        this->Granularity = true;
        this->DPL = privilege;
        this->S = !isSystemSegment;
        this->L = (!isSystemSegment) && Executable();
    }

    void setAddress(uint32_t BaseAddress)
    {
        LowerBaseAddress = BaseAddress & 0xFFFF;
        MiddleBaseAddress = (BaseAddress >> 16) & 0xFF;
        HighBaseAddress = (BaseAddress >> 24) & 0xFF;
    }
    // argument Limit must be below 0x100000.
    void setLimit(uint32_t Limit)
    {
        LowerLimit = Limit & 0xFFFF;
        HighLimit = (Limit >> 16) & 0x0F;
    }
    bool accessed()
    {
        return Type & AccessBit;
    }
    bool RW()
    {
        return Type & RWbit;
    }
    bool DC()
    {
        return Type & DCbit;
    }
    bool Executable()
    {
        return Type & Executablebit;
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