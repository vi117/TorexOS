#pragma once 
#include <stdint.h>
#include <time/duration.h>
#include <drivers/driver.h>

namespace ps
{
class SchedulerTimer
{
public:
    //set duration. set variable now to 0.
    virtual void expired_after(util::millisecond_t){}
    //set raw count
    virtual void expired_count(uint64_t){}

    virtual ~SchedulerTimer() noexcept {}
};
} // namespace ps
