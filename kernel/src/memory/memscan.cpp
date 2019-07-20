#include <memory/memscan.h>
//#include <raw_ostream.h>

static const char * memory_type_name[]={
    "Free",
    "Reserved",
    "ACPI_reclaim",
    "ACPI_NVS",
    "Bad"
};

void memory::printMemoryMap(text::raw_ostream & os){
    uint32_t length = *phys_addr_t(0x610-4).to_ker().to_ptr<uint32_t>();
    auto dscr = phys_addr_t(0x610).to_ker().to_ptr<memory::AddressRangeDescriptor>();
    os << length/24 << "\n";
    for(size_t i = 0; i < length; i+= sizeof(*dscr))
    {    
        os << (void *)dscr->base.address <<" "<< dscr->length << " "
        << memory_type_name[static_cast<int>(dscr->type)-1] << "\n";
        dscr++;
    }
}