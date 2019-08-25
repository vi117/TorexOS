#include<arch/interrupt.h>
#include<pic/pic.h>
#include <memory/mm.h>
#include <vga/vgaout.h>
#include <debug/debug.h>

static constexpr uint8_t irq_start = 32;

void x86_64::IRQ_init()
{
    PIC8259::initialize(irq_start);
    PIC8259::maskInterrupt(1);
}
static x86_64::irq_descriptor irq_desc[16];

void x86_64::register_handler(uint8_t irq,drv::IrqHandler * irqHandler)
{
    irq_desc[irq].actions.push_front(irqHandler);
}

extern "C" void ISR_do_IRQ(uint64_t num)
{
    auto irq = &irq_desc[num - irq_start];
    text::raw_print(80-7,0,"IRQ[  ]");
    char buf[3]={0,};
    buf[0] = num / 10 + '0';
    buf[1] = num % 10 + '0';
    text::raw_print(80-3,0,buf);
    for(auto& it : irq->actions)
    {
        it->topHalf();
    }
    PIC8259::sendEOI(num - irq_start);
}