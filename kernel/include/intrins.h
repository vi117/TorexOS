#include<stdint.h>
#include<memory/address.h>
#include<immintrin.h>

#ifndef _INTRINS_H_
#define _INTRINS_H_

static inline void outbyte(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}
static inline void loadgdtr(phys_addr_t base,uint16_t limit)
{
#pragma pack(push,1)
    struct{
        uint16_t Limit;
    uint64_t BaseAddress;
    uint16_t padding;
    } gdtr = {limit,base,0};
    asm volatile ( "lgdt %0" : : "m"(gdtr));
#pragma pack(pop)
}
static inline void loadtr(uint16_t offset)
{
    asm volatile ( "ltr %0" : : "r"(offset) );
}
static inline void loadidtr(phys_addr_t base,uint16_t limit)
{
#pragma pack(push,1)
    struct{
        uint16_t Limit;
    uint64_t BaseAddress;
    uint16_t padding;
    } idtr = {limit,base,0};
    asm volatile ( "lidt %0" : : "m"(idtr));
#pragma pack(pop)
}
//clear interrupt flag.
static inline void clearinterruptflag(){
    __asm__ __volatile__("cli": : :"memory");
}
//set interrupt flag.
static inline void setinterruptflag(){
    __asm__ __volatile__("sti": : :"memory");
}
static inline void simplebarrior(){
    __asm__ __volatile__("nop": : :"memory");
}
static inline void mfence(){
    __asm__ __volatile__("mfence": : :"memory");
}
#endif