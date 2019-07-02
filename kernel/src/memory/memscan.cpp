#include <memory/memscan.h>
//#include <raw_ostream.h>

static const char * memory_type_name[]={
    "usable",
    "reserved",
    "ACPI_reclaim",
    "ACPI_NVS",
    "Bad"
};

void memory::printMemoryMap(text::raw_ostream & os){
    uint32_t length = *reinterpret_cast<uint32_t *>(phys_to_ker(0x610-4));
    auto dscr = reinterpret_cast<memory::AddressRangeDescriptor *>(phys_to_ker(0x610));
    os << length/24 << "\n";
    for(size_t i = 0; i < length; i+= sizeof(*dscr))
    {    
        os << (void *)dscr->base <<" "<< dscr->length << " "
        << memory_type_name[static_cast<int>(dscr->type)-1] << "\n";
        dscr++;
    }
}