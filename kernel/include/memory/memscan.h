#include <stdint.h>
#include <stddef.h>
#include <memory/address.h>
#include <raw_ostream.h>
#include <compiler.h>

#ifndef _MEMSCAN_H_
#define _MEMSCAN_H_

namespace memory
{
    /*arch*/
    #pragma pack(push, 1)
    struct e820MmapEtnry
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

        bool range_in(phys_addr_t f,phys_addr_t l){
            return (base <= f )
                && (l <= (base + length));
        }
    };
    #pragma pack(pop)
    force_inline size_t e820_mmap_entry_count(){ return *(phys_addr_t(0x610-4).to_ker().to_ptr_of<size_t>());}
    force_inline e820MmapEtnry * e820_mmap_entry(){return phys_addr_t(0x610).to_ker().to_ptr_of<e820MmapEtnry>();}
    void printMemoryMap(text::raw_ostream & r);
} // memory
#endif