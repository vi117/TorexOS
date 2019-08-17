#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>
#include <arch/mminit.h>
#include <memory/mm.h>

#include <drivers/ps2/keyboard.h>
#include <keyboard/scancode.hpp>
#include <intrins.h>


[[noreturn]]
int main()
{
    text::vga_initialize(0,19);
    vga_ostream out;
    out << "Initialize GDT, TSS and IDT.\n";
    x86_64::init_all();
    _init();
    out << "start memory collecting...\n";
    memory::init();
    memory::printMemoryMap(out);
    memory::printMemoryCount(out);
    ps2::state state;
    state.activate();
    
    void * s = memory::kmalloc(32);
    memory::kfree(s);
    void * t = memory::kmalloc(32);
    out << s << " and " << t << "\n";

    clearinterruptflag();
    while(1){
        if(state.isAviliable()){
        auto sc = state.getScanCode();
        if(!sc.code.up)
            out << key::KeyMappingTable[sc.code.name].NormalCode;
        }
        /* do nothing. */
    }
}
