#pragma once
#include<algo/utility.h>
#include<algo/new.h>

namespace util
{
    template<typename Ty>
    union delay_constructor
    {
        Ty value;
        char dummy;
        constexpr delay_constructor():dummy(){}
        
        template<typename ... Args>
        void initialize(Args && ... arg)
        {
            new(addressof(value)) Ty (forward<Args>(arg)...);
        }
    };
       
} // util
