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
    uint32_t length = e820_mmap_entry_count();
    auto dscr = e820_mmap_entry();
    os << length << "\n";
    for(size_t i = 0; i < length; i++)
    {    
        os << (void *)dscr->base.address <<" "<< dscr->length << " "
        << memory_type_name[static_cast<int>(dscr->type)-1] << "\n";
        dscr++;
    }
}