#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>
#include <arch/mminit.h>
#include <memory/mm.h>

[[noreturn]]
int main()
{
    text::vga_initialize(0,19);
    _init();
    vga_ostream out;
    out << "Initialize GDT, TSS and IDT.\n";
    x86_64::init_all();
    //memory::printMemoryMap(out);
    memory::init();
    while(1){
        /* do nothing. */
    }
}