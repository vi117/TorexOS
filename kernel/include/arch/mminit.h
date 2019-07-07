#include<stdint.h>
#include<memory/address.h>

#ifndef _MMINIT_H_
#define _MMINIT_H_
/*
ia32e
amd64
x86_64
*/
namespace x86_64
{
    void gdt_and_tss_init();
    void idt_init();
    void register_ISR(uint8_t index,void(*ptr)());
    void unregister_ISR(uint8_t);
    /*void init_all(){
        gdt_and_tss_init();
    }*/
}

#endif