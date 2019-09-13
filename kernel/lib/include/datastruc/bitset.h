#include <stddef.h>
#include <stdint.h>
#include <math/ilog2.h>

namespace util
{
    struct bitset_view{
        uint64_t * data;
        size_t index;
        inline bool operator=(bool value){
            if (value){
                data[(index >> 6)] |= (1<< (index&0x3f));
            }
            else{
                data[(index >> 6)] &= ~(1<< (index&0x3f));
            }
            return value;
        }
        inline operator bool() const {
            return data[index >> 6] & (1<<(index & 0x3f));
        }
    };
    template<size_t N>
    struct bitset
    {
        static constexpr size_t size = (N - 1) / 64 + 1;
        uint64_t data[size];
        constexpr bool operator[](size_t index) const
        {
            return data[index >> 6] & (1<<(index & 0x3f));
        }
        bitset_view operator[](size_t index)
        {
            return bitset_view{data,index};
        }
    };
    
    size_t find_first_set(uint8_t * data, size_t size)
    {
        size_t i = 0;
        uint64_t * const d = reinterpret_cast<uint64_t *>(data);
        while (d[i] == 0 || i < size) i++;
        //assume little endian.
        const auto r = i*8 + math::ctz64(d[i]);
        return r < size*8 ? size*8 : r;
    }
    size_t find_first_unset(uint8_t * data, size_t size)
    {
        size_t i = 0;
        uint64_t * const d = reinterpret_cast<uint64_t *>(data);
        while (d[i] == (uint64_t)(-1) || i < size) i++;
        //assume little endian.
        const auto r = i*8 + math::ctz64(!d[i]);
        return r < size*8 ? size*8 : r;
    }
} // util
