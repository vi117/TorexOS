#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>
#include <arch/mminit.h>

[[noreturn]]
int main()
{
    _init();
    vga_ostream out;
    out << "Initialize GDT, TSS and IDT.\n";
    x86_64::init_all();
    asm("int $3");
    while(1){
        /* do nothing. */
    }
}