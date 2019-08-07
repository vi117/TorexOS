#include <stdint.h>
#include <stddef.h>

#ifndef _ADDRESS_H_
#define _ADDRESS_H_
inline namespace address_space
{
template <typename DerivedType>
struct address_base
{
    uintptr_t address;

    using address_t = DerivedType;
    using diff_t = size_t;

    constexpr explicit address_base() : address(0) {}
    constexpr explicit address_base(nullptr_t) : address(0) {}
    constexpr explicit address_base(uintptr_t ptr) : address(ptr) {}
    constexpr address_t operator+(diff_t rhs) const { return address_t(address + rhs); }
    address_t &operator+=(diff_t rhs)
    {
        address += rhs;
        return static_cast<address_t &>(*this);
    }
    constexpr address_t operator-(diff_t rhs) const { return address_t(address - rhs); }
    constexpr diff_t operator-(address_t rhs) const { return (address - rhs.address); }
    address_t &operator-=(diff_t rhs)
    {
        address -= rhs;
        return static_cast<address_t &>(*this);
    }
    inline constexpr bool operator==(address_t rhs) const{return address == rhs.address;}
    inline constexpr bool operator!=(address_t rhs) const{ return address != rhs.address;}
    inline constexpr bool operator<(address_t rhs) const{return address < rhs.address;}
    inline constexpr bool operator>(address_t rhs) const{return address > rhs.address;}
    inline constexpr bool operator>=(address_t rhs) const{return address >= rhs.address;}
    inline constexpr bool operator<=(address_t rhs) const{return address <= rhs.address;}
    
    /*must be alignment expressed as 2^n.*/
    constexpr address_t align(uint64_t alignment) const{
        return address_t((address + alignment - 1) & ~(alignment - 1));
    }
};
struct ker_addr_t;
struct phys_addr_t;

struct phys_addr_t : public address_base<phys_addr_t>
{
    using BaseType = address_base;
    using BaseType::address_base;
    constexpr ker_addr_t to_ker() const;
};
struct ker_addr_t : public address_base<ker_addr_t>
{
    using BaseType = address_base;
    using BaseType::address_base;

    explicit ker_addr_t(void *ptr) : BaseType(reinterpret_cast<uintptr_t>(ptr)) {}
    template <typename Ty>
    explicit ker_addr_t(Ty *ptr) : ker_addr_t(static_cast<void *>(ptr)) {}

    constexpr phys_addr_t to_phys() const;

    inline void *to_void_ptr() const
    {
        return reinterpret_cast<void *>(address);
    }
    template <typename Ty>
    Ty *to_ptr_of() const
    {
        return static_cast<Ty *>(to_void_ptr());
    }
};
inline constexpr ker_addr_t phys_addr_t::to_ker() const
{
    return ker_addr_t(address | 0xffff800000000000);
}
inline constexpr phys_addr_t ker_addr_t::to_phys() const
{
    return phys_addr_t(address & (~0xffff800000000000));
}

inline constexpr unsigned long long operator"" _KB(unsigned long long a)
{
    return a * 1024;
}
inline constexpr unsigned long long operator"" _MB(unsigned long long a)
{
    return a * 1024 * 1024;
}
inline constexpr unsigned long long operator"" _GB(unsigned long long a)
{
    return a * 1024 * 1024 * 1024;
}

} // namespace address
#endif
