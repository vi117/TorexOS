#pragma once
#include<memory/address.h>
#include<raw_ostream.h>

namespace memory
{
    void init();
    unsigned getFreeMemoryBlockCount();
    unsigned getTotalMemoryBlockCount();
    void printMemoryCount(text::raw_ostream & os);
    bool is_mapped_in_kernel(phys_addr_t);
    /*Mapping 1GB*/
    void mmap_kernel(phys_addr_t);

    void allocator_init();
    void slab_allocator_init();
} // memory