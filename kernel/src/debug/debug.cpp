#include <debug/debug.h>
#include <vga/vgaout.h>
#include <intrins.h>

void errstream::write_impl(const char * str,size_t n){
    text::vga_write(str,n);
}

static errstream err;
errstream debug;
/*
struct stackframe {
  struct stackframe* ebp;
  void * eip;
};
void StackTrace(void * ebp ,unsigned int MaxFrames)
{
    stackframe *stk = (stackframe *)ebp;
    err << "Stack trace:\n";
    for(size_t frame = 0; stk != nullptr && frame < MaxFrames; ++frame)
    {
        // Unwind to previous stack frame
        err << stk->eip << "\n";
        stk = stk->ebp;
    }
}*/

void panic_impl(const char * message,
                    const char * file,
                    const char * function,
                    unsigned long line){
    clearinterruptflag();
    err << message
        << "\nfile :" <<file
        << "\nfunc :" <<function
        << "\nline :" <<line;
    err << "\ncaller address : " << __GET_CALLER(0);
    for(;;);
}
#include <arch/context.h>
void panicInException(int handler_num,const char *message){
    clearinterruptflag();
    err << message<< " at " << x86_64::getContextInException()->rip;
    err << " occured in " << handler_num << " handler";
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
