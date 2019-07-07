#include <arch/mminit.h>
#include <arch/GDT.h>
#include <arch/IDT.h>
#include <arch/TSS.h>
#include <stddef.h>
#include <memory/address.h>
#include <intrins.h>
#include <stringutil.h>

extern char IST_END;
void x86_64::gdt_and_tss_init(){
    auto gdt_addr = 0x6000;
    auto gdt_length = sizeof(GDT::Entry8)*3 + sizeof(GDT::Entry16);

    GDT::Entry8 * gdt = reinterpret_cast<GDT::Entry8 *>(phys_to_ker(gdt_addr));
    gdt[0].setZero();
    gdt[1].setLongmodeDefualt(false,GDT::RWbit | GDT::Executablebit, 0);
    gdt[2].setLongmodeDefualt(false,GDT::RWbit,0);

    auto gdt16 = static_cast<GDT::Entry16 *>(&gdt[3]);
    gdt16->setLongmodeDefualt(true,GDT::TSS_32bit,0);
    gdt16->setAddress(gdt_addr + gdt_length+sizeof(GDT::Entry16));
    gdt16->setLimit(sizeof(TSS) - 1);

    auto tss = static_cast<TSS *>(static_cast<void *>(&gdt16[2]));
    text::memset(tss,0,sizeof(TSS));
    tss->IOMapDisable();
    tss->IST[0] = reinterpret_cast<uintptr_t>(&IST_END);

    loadgdtr(gdt_addr,gdt_length);
    loadtr(sizeof(GDT::Entry8)*3);
}