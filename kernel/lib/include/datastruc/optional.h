#pragma once
#include<algo/base.h>
#include<algo/utility.h>

namespace util
{
struct trivial_init_t{};

struct nullopt_t{};
nullopt_t nullopt;

template<typename Ty>
union optional_storage
{
    unsigned char dummy;
    Ty value;
    explicit constexpr optional_storage( trivial_init_t ) noexcept : dummy() {};
    
    template <typename ... Args>
    explicit constexpr optional_storage( Args&&... args ) : value(forward<Args>(args)...) {}
};

template<typename Ty>
struct optional
{
    using value_type = Ty;
    constexpr optional(nullopt_t) noexcept :mOk(false),mValue(trivial_init_t{}){}
    template<typename ... Args>
    constexpr optional(Args&&... args):mOk(true), mValue(forward<Args>(args)...){}
    constexpr optional(const optional & other ) = default;
    constexpr optional(optional && other):mOk(other.mOk),mValue(move(other.mValue)){
        other.mOk = false;
    }

    constexpr explicit operator bool() const noexcept{return mOk;}
    constexpr bool has_value() const noexcept{return mOk;}

    constexpr value_type & value() & { return mValue.value;};
    constexpr const value_type & value() const & {return mValue.value;}
    constexpr value_type&& value() && {return mValue.value;}
    constexpr const value_type&& value() const &&{ return mValue.value;}

    template<typename U>
    constexpr value_type value_or(U&& default_value) const& {
        return this->has_value()
        ? (this->value())
        : static_cast<value_type>(forward<U>(default_value));
    }
    template<typename U>
    constexpr value_type value_or(U&& default_value) && {
        return this->has_value()
        ? (move(this->value()))
        : static_cast<value_type>(forward<U>(default_value));
    }

    void reset(){

    }

    ~optional(){
        if(mOk)
            mValue.value.value_type::~value_type();
    }
    private:

    bool mOk;
    alignas(value_type) optional_storage<value_type> mValue;
};
} // util

