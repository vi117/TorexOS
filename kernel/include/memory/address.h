#include<stdint.h>

#ifndef _ADDRESS_H_
#define _ADDRESS_H_

using phys_addr_t = uint64_t;

inline constexpr void * phys_to_ker(phys_addr_t addr){
    return reinterpret_cast<void *>(addr|0xffff800000000000);
}

#endif
