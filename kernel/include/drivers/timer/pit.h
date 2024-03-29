#pragma once
#include <stdint.h>
#include <process/sched_timer.h>

namespace PIT
{
enum
{
    frequency = 1193182,
    port_Control = 0x43,
    port_Counter0 = 0x40,
};

void initialize(uint16_t count, bool periodic);
uint16_t ReadCounter0();
//================================

void waitUsingDirectPIT(uint16_t);
void waitMs(unsigned long millisecond);

//================================

inline constexpr auto msToCount(uint16_t x) -> uint16_t
{
    return (frequency * x / 1000);
}
inline constexpr auto usToCount(uint16_t x) -> uint16_t
{
    return (frequency * x / 1000000);
}

void init_for_sched();
//it must be used after init_for_sched.
ps::SchedulerTimer * get_PIT();
} // namespace PIT