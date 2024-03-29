#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>
#include <arch/mminit.h>
#include <memory/mm.h>

#include <pic8259/pic8259.h>
#include <arch/interrupt.h>

#include <drivers/ps2/keyboard.h>
#include <keyboard/scancode.hpp>
#include <intrins.h>

#include <acpi/rsdt.h>
#include <drivers/timer/cmos.h>
#include <drivers/timer/pit.h>

#include <process/process.h>
#include <process/scheduler.h>

#include <debug/debug.h>

static void loop();

static vga_ostream out;

[[noreturn]]
int main()
{
    text::vga_initialize(0,19);
    vga_ostream out;
    out << "Initialize GDT, TSS and IDT.\n";
    x86_64::init_all();
    out << "Memory Entry Count : ";
    _init();
    if(!acpi::prepareTable())
        panic("couldn't load ACPI.");
    //memory::printMemoryMap(out);
    out << "start memory collecting...\n";
    memory::init();
    memory::printMemoryCount(out);

    /*CMOS::CMOS_RTC t;
    t.update();
    out << t.year <<"year " << (int)t.month << "month "
     << (int)t.dayOfMonth<<"day " << t.dayOfWeekToCString()
     <<" " << (int)t.hour<<"h "
     << (int)t.minute<<"m " << (int)t.second << "s \n";*/
    x86_64::IRQ_init();
    //PIT::initialize(PIT::msToCount(1),true);
    PIT::init_for_sched();
    ps::start_scheduling(loop);
}

void loop()
{
    ps2::Keyboard keyboard;
    bool kbd_init= keyboard.initialize();
    if(!kbd_init) out << "failed to initialize ps/2 keyboard.\n";
    /*{
        void * s = memory::kmalloc(32);
        void * v = memory::kmalloc(32);
        memory::kfree(s);
        void * t = memory::kmalloc(32);
        out << s << " and " << t << "\n";
        memory::kfree(v);
    }*/
    //clearinterruptflag();
    acpi::printSDTs(out);
    
    key::ScanCode sc;
    uint8_t buf[4];
    while(true){
        auto length = keyboard.read(buf,4);
        if(length > 0){
            sc.data = buf[0];
            //out << " "<< (int)buf[0];
            if(!sc.code.up)
                out << key::KeyMappingTable[sc.code.name].NormalCode;
        }
    }
}