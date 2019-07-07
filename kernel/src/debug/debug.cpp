#include <debug/debug.h>
#include <vga/vgaout.h>
#include <intrins.h>

void errstream::write_impl(const char * str,size_t n){
    text::vga_write(str,n);
}

static errstream err;
errstream debug;

void panic(const char * message){
    clearinterruptflag();
    err << message;
    for(;;);
}

void assert_impl(const char * message,
                    const char * file,
                    const char * function,
                    unsigned long line){
    clearinterruptflag();
    err << message
        << "\nfile :" <<file
        << "\nfunc :" <<function
        << "\nline :" <<line;
    for(;;);
}
