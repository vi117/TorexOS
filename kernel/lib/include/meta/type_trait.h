#ifndef _META_TYPE_TRAIT_H_
#define _META_TYPE_TRAIT_H_
namespace util
{

template<typename Ty,Ty v>
struct integral_constant{
    static constexpr Ty  value = v;
    typedef Ty value_type;
    typedef integral_constant type;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template<bool val>
using bool_constant = integral_constant<bool,val>; 

template<bool value, class Ty = void>
struct enable_if {};
 
template<class Ty>
struct enable_if<true, Ty> { using type = Ty; };

} // util
#endif