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
    err << "\ncaller address : " << __GET_CALLER(0);
    for(;;);
}
#include <arch/context.h>
void panicInException(int handler_num,const char *message){
    clearinterruptflag();
    err << message;
    err << " occured in " << handler_num << " handler";
    err << "\ncaller address : " << x86_64::getContextInException()->rip;
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
        << "\nline :" <<line
        << "\ncaller address : " << __GET_CALLER(0);
    for(;;);
}
