#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>
#include <arch/mminit.h>

[[noreturn]]
int main()
{
    _init();
    vga_ostream out;
    out << "Init Gdt and Tss.\n";
    x86_64::gdt_and_tss_init();
    while(1){
        /* do nothing. */
    }
}