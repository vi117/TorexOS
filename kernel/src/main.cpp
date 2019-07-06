#include <vga/vgaout.h>
#include <cpp_constructor.h>
#include <memory/memscan.h>

[[noreturn]]
int main()
{
    _init();
    vga_ostream out;
    out << "scan memory.\n";
    
    memory::printMemoryMap(out);
    while(1){
        /* do nothing. */
    }
}