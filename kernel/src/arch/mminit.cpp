#include <arch/mminit.h>
#include <arch/GDT.h>
#include <arch/IDT.h>
#include <arch/TSS.h>
#include <stddef.h>
#include <memory/address.h>
#include <intrins.h>
#include <stringutil.h>

#include <debug/debug.h>

extern char IST_END;
static constexpr int gdt_entry8_num=3;
static constexpr int gdt_entry16_num=1;
void x86_64::gdt_and_tss_init(util::LinearAllocator<uint8_t> & allocator){
    auto gdt_length = sizeof(GDT::Entry8)*3 + sizeof(GDT::Entry16);
    auto gdt_addr = (reinterpret_cast<ker_addr_t>
        (allocator.allocate(gdt_length)));
    GDT::Entry8 * gdt = reinterpret_cast<GDT::Entry8 *>(gdt_addr);
    gdt[0].setZero();
    gdt[1].setLongmodeDefualt(false,GDT::RWbit | GDT::Executablebit, 0);
    gdt[2].setLongmodeDefualt(false,GDT::RWbit,0);

    auto gdt16 = static_cast<GDT::Entry16 *>(&gdt[3]);
    auto tss = reinterpret_cast<TSS *>(allocator.allocate(sizeof(TSS)));

    gdt16->setLongmodeDefualt(true,GDT::TSS_32bit,0);
    gdt16->setAddress(reinterpret_cast<uintptr_t>(tss));
    gdt16->setLimit(sizeof(TSS) - 1);

    text::memset(tss,0,sizeof(TSS));
    tss->IOMapDisable();
    tss->IST[0] = reinterpret_cast<uintptr_t>(&IST_END);

    loadgdtr(ker_to_phys(gdt_addr),gdt_length-1);
    loadtr(sizeof(GDT::Entry8)*3);
}

extern IDT_Handler interrupt_handler_vector[128];

void x86_64::idt_init(util::LinearAllocator<uint8_t> & allocator){
    constexpr auto idt_count = 128;
    auto idt_addr = static_cast<IDT *>(
        static_cast<void *>(allocator.allocate(sizeof(IDT)*idt_count)));
    for(size_t i = 0 ;i < idt_count; i++ ){
        IDT temp;
        temp.IST = 1;
	    temp.Present = true;
	    temp.DPL = 0;
	    temp.Type = ISRType::interrupt_gate_32bit;
        temp.SegmentSelector = 0x8;/*kernel code segment*/
        temp.SetHandler(interrupt_handler_vector[i]);
        idt_addr[i] = temp;
    }
    loadidtr(ker_to_phys((ker_addr_t)idt_addr),idt_count * sizeof(IDT) - 1);
}

void x86_64::init_all(){
    util::LinearAllocator<uint8_t> allocator
        (reinterpret_cast<uint8_t *>(phys_to_ker(0x6000)),
        reinterpret_cast<uint8_t *>(phys_to_ker(0x7000)));
    gdt_and_tss_init(allocator);
    idt_init(allocator);
}

extern "C" void ISRCommon(int Handler_num){
    panicInException(Handler_num,"Not ready interrupt");
}