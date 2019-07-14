#include<stdint.h>
#include<memory/address.h>
#include<alloc_policy/linear_allocator.h>

#ifndef _MMINIT_H_
#define _MMINIT_H_
/*
ia32e
amd64
x86_64
*/
namespace x86_64
{
    // @param allocator return linear address.
    void gdt_and_tss_init(util::LinearAllocator<uint8_t> & allocator);
    void idt_init(util::LinearAllocator<uint8_t> & allocator);
    void register_ISR(uint8_t index,void(*handler)());
    void unregister_ISR(uint8_t);
    void init_all();
}

#endif