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
struct type_identity
{
    using type = Ty;
};
namespace detail {

template <class T>
auto try_add_lvalue_reference(int) -> type_identity<T&>;
template <class T>
auto try_add_lvalue_reference(...) -> type_identity<T>;
 
template <class T>
auto try_add_rvalue_reference(int) -> type_identity<T&&>;
template <class T>
auto try_add_rvalue_reference(...) -> type_identity<T>;
 
} // namespace detail
 
template <class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};
 
template <class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};

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

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

template< bool B, class T = void >
using enable_if_t = typename enable_if<B,T>::type;


} // namespace util
