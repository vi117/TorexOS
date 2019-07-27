#pragma once
#include<algo/utility.h>

namespace util
{
    template<typename Ty>
    union delay_constructor
    {
        char dummy;
        Ty value;
        constexpr delay_constructor():dummy(){}
        
        template<typename ... Args>
        void initialize(Args && ... arg)
        {
            new(addressof(value)) (forward<Args>(arg)...);
        }
    };
       
} // util
