#pragma once
#include <acpi/rsdt.h>

namespace acpi
{
    #pragma pack(push,1)
    enum class APICEntryType : uint8_t
    {
        LocalApic = 0,
        IOApic = 1,
        InterruptSourceOverride = 2,
        NMI = 4,
        LocalApicAddressOverride = 5
    }
    struct APICRecordHeader
    {
        APICEntryType Type;
        uint8_t Length;
    };
    struct LocalApicEntry : public APICRecordHeader
    {
        uint8_t AcpiProcessorID;
        uint8_t ApicID;
        uint32_t Flags; // 1 = Processor Enabled
    };
    struct IOApicEntry : public APICRecordHeader
    {
        uint8_t IOApicID;
        uint8_t Reserved;
        uint32_t IOApicAddress;
        uint32_t GlobalSystemInterruptBase;
    };
    struct InterruptSourceOverrideEntry : public APICRecordHeader
    {
        uint8_t BusSource;
        uint8_t IrqSource;
        uint32_t GlobalSystemInterrupt;
        uint16_t Flags;
    };
    struct NMIEntry : public APICRecordHeader
    {
        uint8_t AcpiProcessorID;
        uint16_t Flags;
        uint8_t LINT; // 0 or 1
    };
    struct LocalApicAddressOverrideEntry : public APICRecordHeader
    {
        uint16_t reserved;
        uint64_t LocalApicPhysicalAddress;
    };

    struct ApicRecordIterator
    {
        ApicRecordIterator & operator++()
        {
            pos += ((APICRecordHeader *)pos)->Length;
            return *this;
        }
        ApicRecordIterator & operator++(int){return operator++();}
        APICRecordHeader * operator->(){return (APICRecordHeader *)pos;}
        APICRecordHeader * get(){return (APICRecordHeader *)pos;}
        bool operator==(const ApicRecordIterator & rhs){return pos == rhs.pos;}
        bool operator!=(const ApicRecordIterator & rhs){return pos != rhs.pos;}
    private:
        uint8_t * pos;
    };

    struct MADT : public SDTHeader
    {
        uint32_t ApicBaseAddress;
        uint32_t Flags;//1 = Dual 8259 Legacy PICs Installed.
        uint8_t Record[0];

        ApicRecordIterator begin()
        {
            return ApicRecordIterator{&Record[0]};
        }
        ApicRecordIterator end()
        {
            return ApicRecordIterator{&Record[this->Length-32]};
        }
    };
    #pragma pack(pop)
    inline MADT * findMADT(){
        return (MADT *)findSDT("APIC");
    }
} // namespace acpi
