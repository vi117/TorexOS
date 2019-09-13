#pragma once
#include<stdint.h>

namespace ps
{
    using pid_t = int;
    
    //dummy
    pid_t pid_alloc();
    void pid_free(pid_t);
    
} // namespace ps
