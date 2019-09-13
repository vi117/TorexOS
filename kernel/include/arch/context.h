#include <stdint.h>
#include <compiler.h>

#ifndef _CONTEXT_H_
#define _CONTEXT_H_
extern uint8_t IST_END;
namespace x86_64
{
#pragma pack(push,1)
struct cpuContext
{
    uint64_t gs;   //0
    uint64_t fs;   //1
    uint64_t es;   //2
    uint64_t ds;   //3
    uint64_t r15;  //4
    uint64_t r14;  //5
    uint64_t r13;  //6
    uint64_t r12;  //7
    uint64_t r11;  //8
    uint64_t r10;  //9
    uint64_t r9;   //10
    uint64_t r8;   //11
    uint64_t rsi;  //12
    uint64_t rdi;  //13
    uint64_t rdx;  //14
    uint64_t rcx;  //15
    uint64_t rbx;  //16
    uint64_t rax;  //17
    uint64_t *rbp; //stack base pointer
    void *rip;     //instruction pointer
    uint64_t cs;   //code segment
    uint64_t RFLAG;
    uint64_t *rsp; //stack pointer
    uint64_t ss;   //stack segment
};

struct mmRegister
{
    uint64_t data1;
    uint16_t data2;
    uint16_t reserve1;
    uint32_t reserve2;
};
struct xmmRegister
{
    uint64_t high;
    uint64_t low;
};

struct fpuContext
{
    uint16_t fcw;
    uint16_t fsw;
    uint8_t ftw;
    uint8_t nullSection;
    uint16_t fop;
    uint64_t fpuIP;
    uint64_t fpuDP;
    uint32_t MXCSR;
    uint32_t MXCSR_MASK;
    mmRegister mm[8];
    xmmRegister xmm[16];
    uint64_t reserved[2 * 6];
};
#pragma pack(pop)
force_inline cpuContext * getContextInException(){
    return reinterpret_cast<cpuContext *>(&IST_END - sizeof(cpuContext));
}
} // namespace x86_64
//it do not handle interrupt occurasnce.
extern "C" void SwitchContext(x86_64::cpuContext * prev,x86_64::cpuContext * next);
namespace x86_64{
force_inline void switch_to(cpuContext * prev,cpuContext * next){
    SwitchContext(prev,next);
}
}
#endif