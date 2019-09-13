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
struct remove_reference{using type = Ty;};
template <typename Ty>
struct remove_reference<Ty &>{using type = Ty;};
template <typename Ty>
struct remove_reference<Ty &&>{using type = Ty;};

namespace detail
{
template <class T>
auto try_add_pointer(int) -> type_identity<typename remove_reference<T>::type *>;
template <class T>
auto try_add_pointer(...) -> type_identity<T>;
}
template <class T>
struct add_pointer : decltype(detail::try_add_pointer<T>(0)) {};

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

template<typename T>
struct is_integral : public false_type{};
#define DEFINT(x) template<> struct is_integral<x> : public true_type{}
DEFINT(short);
DEFINT(unsigned short);
DEFINT(int);
DEFINT(long);
DEFINT(unsigned int);
DEFINT(unsigned long);
DEFINT(long long);
DEFINT(unsigned long long);
#undef DEFINT
template< class T >
struct is_arithmetic : integral_constant<bool,is_integral<T>::value > {};
namespace detail {
template<typename T,bool = is_arithmetic<T>::value>
struct is_unsigned : integral_constant<bool, T(0) < T(-1)> {};
template<typename T>
struct is_unsigned<T,false> : false_type {};
} // namespace detail
template<typename T>
struct is_unsigned : detail::is_unsigned<T>::type {};


} // namespace util
