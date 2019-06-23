#include <vga/vgaout.h>
#include <cpp_constructor.h>

int main()
{
    call_constructor();
    text::initialize();
    text::puts("Hello, World!\n");
    text::puts("aaaa");
    
    while(1){
        /* do nothing. */
    }
}