#pragma once
#include<memory/address.h>
#include<raw_ostream.h>
#include<memory/pageframe.h>

#include<algo/new.h>

namespace memory
{
    void init();

    unsigned getFreeMemoryBlockCount();
    unsigned getTotalMemoryBlockCount();
    void printMemoryCount(text::raw_ostream & os);
    
    PageDescriptor * alloc_pages(PageType pt,order_t order);
    void free_pages(PageDescriptor * pd);

    ker_addr_t alloc_pages_kma(PageType pt, size_t sz);

    void allocator_init();
    
    void * kmalloc(size_t);
    void kfree(void *);
} // memory

namespace mm = memory;