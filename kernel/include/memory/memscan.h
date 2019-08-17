#include <stdint.h>
#include <stddef.h>
#include <memory/address.h>
#include <raw_ostream.h>
#include <compiler.h>

#ifndef _MEMSCAN_H_
#define _MEMSCAN_H_

namespace memory
{
/*
• The platform boot firmware returns address ranges describing baseboard memory.
• The platform boot firmware does not return a range description for the memory mapping of
PCI devices, ISA Option ROMs, and ISA Plug and Play cards because the OS has mechanisms
available to detect them.
• The platform boot firmware returns chip set-defined address holes that are not being used by
devices as reserved.
• Address ranges defined for baseboard memory-mapped I/O devices, such as APICs, are
returned as reserved.
• All occurrences of the system platform boot firmware are mapped as reserved, including the
areas below 1 MB, at 16 MB (if present), and at end of the 4-GB address space.
• Standard PC address ranges are not reported. For example, video memory at A0000 to BFFFF
physical addresses are not described by this function. The range from E0000 to EFFFF is specific
to the baseboard and is reported as it applies to that baseboard.
• All of lower memory is reported as normal memory. The OS must handle standard RAM
locations that are reserved for specific uses, such as the interrupt vector table (0:0) and the
platform boot firmware data area (40:0).
*/

    #pragma pack(push, 1)
    /* system address map entry.
    *you can read system address map in ACPI specification*/
    struct e820MmapEntry
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
        Unusable,
        Disabled,
        PersistentMemory,
        //others are undefined or OEM defined.
        };

        bool range_in(phys_addr_t f,phys_addr_t l) const{
            return (base <= f )
                && (l < (base + length));
        }
    };
    #pragma pack(pop)
    force_inline size_t e820_mmap_entry_count(){ return *(phys_addr_t(0x610-4).to_ker().to_ptr_of<size_t>());}
    force_inline e820MmapEntry * e820_mmap_entry(){return phys_addr_t(0x610).to_ker().to_ptr_of<e820MmapEntry>();}
    void printMemoryMap(text::raw_ostream & r);
} // memory
#endif