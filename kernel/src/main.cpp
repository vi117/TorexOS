#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>
#include <arch/mminit.h>
#include <memory/mm.h>

[[noreturn]]
int main()
{
    text::vga_initialize(0,19);
    vga_ostream out;
    out << "Initialize GDT, TSS and IDT.\n";
    x86_64::init_all();
    _init();
    //memory::printMemoryMap(out);
    out << "start memory collecting...\n";
    memory::init();
    memory::printMemoryCount(out);
    for(;;);
    while(1){
        /* do nothing. */
    }
}