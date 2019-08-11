#include <stddef.h>
#include <stdint.h>
#include <math/ilog2.h>

namespace util
{
    struct bitmap_view{
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
    size_t find_first_set(uint8_t * data, size_t size)
    {
        size_t i = 0;
        uint64_t * const d = reinterpret_cast<uint64_t *>(data);
        while (d[i] == 0 || i < size){ i++;}
        //assume little endian.
        const auto r = i*8 + math::ctz64(d[i]);
        return r < size*8 ? size*8 : r;
    }
    size_t find_first_unset(uint8_t * data, size_t size)
    {
        size_t i = 0;
        uint64_t * const d = reinterpret_cast<uint64_t *>(data);
        while (d[i] == (uint64_t)(-1) || i < size){ i++;}
        //assume little endian.
        const auto r = i*8 + math::ctz64(!d[i]);
        return r < size*8 ? size*8 : r;
    }
} // util
