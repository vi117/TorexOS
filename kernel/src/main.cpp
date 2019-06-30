#include <vga/vgaout.h>
#include <cpp_constructor.h>

int main()
{
    _init();
    vga_ostream out;
    out << "hello, world!";
    
    while(1){
        /* do nothing. */
    }
}