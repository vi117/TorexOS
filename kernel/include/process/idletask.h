#pragma once
#include <process/process.h>

namespace ps
{
    void idle_work();
    Task * createIdleTask();
} // namespace ps
