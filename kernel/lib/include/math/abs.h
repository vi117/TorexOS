#pragma once
namespace util
{
    namespace math
    {
        //Ty is primitive type and signed.
        //template<typename Ty>
        //constexpr Ty abs(Ty a){
        //    return a < 0 ? -a: a;
        //}
        #define def_abs(ty) constexpr ty abs(ty a){return a<0 ? -a: a;}
        def_abs(short)
        def_abs(int)
        def_abs(long)
        def_abs(long long)
        #undef def_abs
    } // namespace math
    
} // namespace util
