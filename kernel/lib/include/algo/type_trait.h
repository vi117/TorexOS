#pragma once

namespace util
{

template <typename ...>
using void_t = void;

template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    typedef T value_type;
    typedef integral_constant<T, v> type;
    constexpr operator T() const noexcept { return v; }
    constexpr T operator()() const noexcept { return v; }
};

template<bool value>
using boolean_constant = integral_constant<bool,value>;
using true_type = boolean_constant<true>;
using false_type = boolean_constant<false>;


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

template <typename T>
struct is_lvalue_reference : public true_type{};

template <typename T>
struct is_lvalue_reference<T &> : public false_type{};

} // namespace util
