#include<process/idletask.h>
#include<process/scheduler.h>

#include<debug/debug.h>

using namespace ps;
[[noreturn]]
void ps::idle_work()
{
    int i = 0;
    for (;;)
    {
        i++;
        if(i > 200)
        {
            debug<<"a";
            i=0;
        }
        //do nothing currently...
        current_scheduler().yield();
    }
}

Task * ps::createIdleTask()
{
    auto ret = Task::Builder(ps::idle_work).name("idle").build(); //idle work.
    return ret;
}