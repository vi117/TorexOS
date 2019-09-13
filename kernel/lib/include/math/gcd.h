#pragma once
#include<math/abs.h>
namespace util
{
    namespace math
    {
        //Ty is unsigned.
        template<typename Ty>
        constexpr Ty gcd(Ty a, Ty b)
        {
            if(a==0)
                return b;
            else if(b==0)
                return a;
            else
                return gcd(b ,a % b);
        }
    } // namespace math
    
} // namespace util
