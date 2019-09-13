#pragma once
#include <stddef.h>

#ifndef _NEW
namespace util
{
struct nothrow_t {};
[[maybe_unused]] static nothrow_t nothrow;
} // namespace util

extern "C++"{
[[nodiscard]]
void* operator new (size_t size);
[[nodiscard]]
void* operator new (size_t size, [[maybe_unused]]const util::nothrow_t& nothrow_value) noexcept;
[[nodiscard]]
void* operator new[] (size_t size);
[[nodiscard]]
void* operator new[] (size_t size, [[maybe_unused]]const util::nothrow_t& nothrow_value) noexcept;


inline void *operator new([[maybe_unused]] size_t, void *ptr) noexcept { return ptr; }
inline void *operator new[]([[maybe_unused]] size_t, void *ptr) noexcept { return ptr; }

inline void operator delete[] ([[maybe_unused]]void* ptr, [[maybe_unused]]void* voidptr2) noexcept{}
inline void operator delete ([[maybe_unused]]void* ptr, [[maybe_unused]]void* voidptr2) noexcept{}


void operator delete (void* ptr) noexcept;
//void operator delete (void* ptr, const util::nothrow_t& nothrow_constant) noexcept;
void operator delete (void* ptr, size_t size) noexcept;
//void operator delete (void* ptr, size_t size,const util::nothrow_t& nothrow_constant) noexcept;

void operator delete[] (void* ptr) noexcept;
//void operator delete[] (void* ptr, [[maybe_unused]]const util::nothrow_t& nothrow_constant) noexcept;
void operator delete[] (void* ptr, size_t size) noexcept;
//void operator delete[] (void* ptr, size_t size,[[maybe_unused]]const util::nothrow_t& nothrow_constant) noexcept;
}
#endif