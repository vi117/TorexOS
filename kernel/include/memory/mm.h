#pragma once
#include<memory/address.h>

namespace memory
{
    void init();
    phys_addr_t alloc_huge_page();
    void free_huge_page(phys_addr_t);
    bool is_mapped_in_kernel(phys_addr_t);
    /*Mapping 1GB*/
    void mmap_kernel(phys_addr_t);

    void allocator_init();
    void slab_allocator_init();
} // memory