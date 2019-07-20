#include <stdint.h>
#include <stddef.h>

#ifndef _ADDRESS_H_
#define _ADDRESS_H_
inline namespace address
{
template <typename DerivedType>
struct address_base
{
    uintptr_t address;

    using address_t = DerivedType;

    explicit address_base() : address(0) {}
    explicit address_base(nullptr_t) : address(0) {}
    explicit address_base(uintptr_t ptr) : address(ptr) {}
    address_t operator+(size_t rhs) const { return address_t(address + rhs); }
    address_t &operator+=(size_t rhs)
    {
        address += rhs;
        return static_cast<address_t &>(*this);
    }
    address_t operator-(size_t rhs) const { return address_t(address - rhs); }
    address_t &operator-=(size_t rhs)
    {
        address -= rhs;
        return static_cast<address_t &>(*this);
    }
};
struct ker_addr_t;
struct phys_addr_t;

struct phys_addr_t : public address_base<phys_addr_t>
{
    using BaseType = address_base;
    using BaseType::address_base;
    const ker_addr_t to_ker() const;
};
struct ker_addr_t : public address_base<ker_addr_t>
{
    using BaseType = address_base;
    using BaseType::address_base;
    template <typename Ty>
    explicit ker_addr_t(Ty *ptr) : ker_addr_t(static_cast<void *>(ptr)) {}

    explicit ker_addr_t(void *ptr) : BaseType(reinterpret_cast<uintptr_t>(ptr)) {}

    const phys_addr_t to_phys() const;

    inline void *to_void_ptr() const
    {
        return reinterpret_cast<void *>(address);
    }
    template <typename Ty>
    Ty *to_ptr() const
    {
        return static_cast<Ty *>(to_void_ptr());
    }
};
inline const ker_addr_t phys_addr_t::to_ker() const
{
    return ker_addr_t(address | 0xffff800000000000);
}
inline const phys_addr_t ker_addr_t::to_phys() const
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

} // namespace address
#endif
