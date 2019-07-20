
#pragma once

namespace util
{
template <typename Ty>
struct remove_reference
{
    typedef Ty type;
};
template <typename Ty>
struct remove_reference<Ty &>
{
    typedef Ty type;
};
template <typename Ty>
struct remove_reference<Ty &&>
{
    typedef Ty type;
};
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

template <class _Ty>
inline _Ty *addressof(_Ty &_Val) noexcept
{ // return address of _Val
    return (reinterpret_cast<_Ty *>(
        (&const_cast<char &>(
            reinterpret_cast<const volatile char &>(_Val)))));
}

} // util