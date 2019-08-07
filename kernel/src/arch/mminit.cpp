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
void x86_64::gdt_and_tss_init(util::LinearAllocator & allocator){
    auto gdt_length = sizeof(GDT::Entry8)*3 + sizeof(GDT::Entry16);
    auto gdt_addr = ker_addr_t(allocator.allocate_bytes(gdt_length));
    GDT::Entry8 * gdt = gdt_addr.to_ptr_of<GDT::Entry8>();
    gdt[0].setZero();
    gdt[1].setLongmodeDefualt(false,GDT::RWbit | GDT::Executablebit, 0);
    gdt[2].setLongmodeDefualt(false,GDT::RWbit,0);

    auto gdt16 = static_cast<GDT::Entry16 *>(&gdt[3]);
    auto tss = static_cast<TSS *>(allocator.allocate_bytes(sizeof(TSS)));

    gdt16->setLongmodeDefualt(true,GDT::TSS_32bit,0);
    gdt16->setAddress(reinterpret_cast<uintptr_t>(tss));
    gdt16->setLimit(sizeof(TSS) - 1);

    text::memset(tss,0,sizeof(TSS));
    tss->IOMapDisable();
    tss->IST[0] = reinterpret_cast<uintptr_t>(&IST_END);

    loadgdtr(gdt_addr.to_phys(),gdt_length-1);
    loadtr(sizeof(GDT::Entry8)*3);
}

extern IDT_Handler interrupt_handler_vector[128];

void x86_64::idt_init(util::LinearAllocator & allocator){
    constexpr auto idt_count = 128;
    auto idt_addr = static_cast<IDT *>(
        static_cast<void *>(allocator.allocate_bytes(sizeof(IDT)*idt_count)));
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
    loadidtr((ker_addr_t(idt_addr)).to_phys(),idt_count * sizeof(IDT) - 1);
}

void x86_64::init_all(){
    auto buffer = phys_addr_t(0x6000).to_ker();
    util::LinearAllocator allocator(
        buffer.to_ptr_of<uint8_t>(), (buffer + 0x1000).to_ptr_of<uint8_t>()
    );
    gdt_and_tss_init(allocator);
    idt_init(allocator);
}

extern "C" void ISRCommon(int Handler_num){
    panicInException(Handler_num,"Not ready interrupt");
}