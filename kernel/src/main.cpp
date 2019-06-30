#include <vga/vgaout.h>
#include <cpp_constructor.h>

class sss{
public:
sss(){
    text::raw_print(0,1,"AAAAA");
}
};
volatile sss s;
int main()
{
    _init();
    text::initialize();
    text::puts("Hello, World!\n");
    text::puts("aaaa");
    
    while(1){
        /* do nothing. */
    }
}