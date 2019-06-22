#include<stdint.h>

#ifndef _ADDRESS_H_
#define _ADDRESS_H_

inline constexpr void * phys_to_ker(void * addr){
    return reinterpret_cast<void *>(reinterpret_cast<uint64_t>(addr)|0xffff800000000000);
}

#endif
