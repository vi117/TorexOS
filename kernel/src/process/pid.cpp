#include<process/pid.h>

//temp.
ps::pid_t ps::pid_alloc()
{
    static int s = 0;
    s++;
    return s;
}
//dummy.
void ps::pid_free(pid_t)
{}