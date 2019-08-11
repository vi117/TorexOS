#pragma once
#include <stdint.h>
#include <stddef.h>

namespace util
{
namespace math
{
#ifdef  __GNUC__
//Count Leading Zeros
inline constexpr size_t clz64(uint64_t n)
{
    return __builtin_clzl(n);
}
//Count Trailing Zeros
inline constexpr size_t ctz64(uint64_t n)
{
    return __builtin_ctzl(n);
}
#else
//Count Leading Zeros
inline constexpr size_t clz64(uint64_t n)
{
    size_t x = 0;
    if (n >> 32)
    {
        x += 32;
        n >>= 32;
    }
    if (n >> 16)
    {
        x += 16;
        n >>= 16;
    }
    if (n >> 8)
    {
        x += 8;
        n >>= 8;
    }
    if (n >> 4)
    {
        x += 4;
        n >>= 4;
    }
    if (n >> 2)
    {
        x += 2;
        n >>= 2;
    }
    x += n >> 1;
    n = (n >> 1) | n & 1;
    x += n;
    return 64 - x;
}
#define process_(one, bits)						\
	if ((x & ((one << (bits))-1)) == 0) { rv += bits; x >>= bits; }

#define process64(bits) process_((UINT64_C(1)), bits)
inline constexpr size_t ctz64(uint64_t x)
{
	size_t rv = 0;
	process64(32);
	process64(16);
	process64(8);
	process64(4);
	process64(2);
	process64(1);
	return rv;
}
#undef process64
#undef process_

#endif
inline constexpr size_t ilog2(uint64_t n){
    return 63 - clz64(n);
}

} // namespace math

} // namespace util
