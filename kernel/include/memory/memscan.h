#include <stdint.h>
#include <stddef.h>
#include <memory/address.h>
#include <raw_ostream.h>

#ifndef _MEMSCAN_H_
#define _MEMSCAN_H_

namespace memory
{
    #pragma pack(push, 1)
    struct AddressRangeDescriptor
    {
        enum class RegionType : uint32_t;
        phys_addr_t base;
        size_t length;
        RegionType type;
        //this must be 1.
        uint32_t reserved;

        enum class RegionType : uint32_t{
        Free = 1,
        Reserved,
        /*Type 3 "ACPI reclaimable" memory regions may be used like (and combined
        * with) normal "available RAM" areas as long as you're finished using the ACPI
        * tables that are stored there (i.e. it can be "reclaimed"). */
        ACPI_reclaimable,
        ACPI_NVS,
        BadMemory
        };
    };
    #pragma pack(pop)
    
    void printMemoryMap(text::raw_ostream & r);
} // memory
#endif