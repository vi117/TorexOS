#pragma once
#include<memory/address.h>
#include<raw_ostream.h>
#include<memory/pageframe.h>

namespace memory
{
    void init();

    unsigned getFreeMemoryBlockCount();
    unsigned getTotalMemoryBlockCount();
    void printMemoryCount(text::raw_ostream & os);
    
    //PageDescriptor * alloc_pages(PageType pt,order_t order);
    //void free_pages(PageDescriptor * pd);
    
    bool is_mapped_in_kernel(phys_addr_t);
    /*Mapping 1GB*/
    void mmap_kernel(phys_addr_t);

    void allocator_init();
    
    //void * kmalloc(size_t);
    //void kfree(void *);
} // memory