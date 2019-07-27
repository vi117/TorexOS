#pragma once
#include <algo/type_trait.h>

namespace util
{

template <typename Ty>
inline typename remove_reference<Ty>::type &&
move(Ty &&arg)
{
    return static_cast<typename remove_reference<Ty>::type &&>(arg);
}

//exchange value stored at left and right.
template <typename Ty> inline void swap(Ty &left, Ty &right)
{
    Ty temp(util::move(left));
    left = util::move(right);
    right = util::move(temp);
}

template<typename T>
inline constexpr T&& forward(typename remove_reference<T>::type & arg) noexcept
{
  return static_cast<T&&>(arg);
}

template<typename T>
inline constexpr T&& forward(typename remove_reference<T>::type && arg) noexcept
{
  static_assert(!is_lvalue_reference<T>::value, "invalid rvalue to lvalue conversion");
  return static_cast<T&&>(arg);
}

template <class Ty>
inline Ty *addressof(Ty & Val) noexcept
{ // return address of _Val
    return (reinterpret_cast<Ty *>(
        (&const_cast<char &>(
            reinterpret_cast<const volatile char &>(Val)))));
}

} // util