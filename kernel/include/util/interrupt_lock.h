#pragma once
#include <intrins.h>

class interrupt_lock{
public:
void lock(){
    previous = static_cast<bool>(get_rflags() & 0x0200);
    clearinterruptflag();
}
void unlock(){
    if(previous)
        setinterruptflag();
}
private:
bool previous;
};