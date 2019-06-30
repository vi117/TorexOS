#include<stdint.h>

#ifndef _ADDRESS_H_
#define _ADDRESS_H_

using phys_addr_t = uintptr_t;
using ker_addr_t = uintptr_t;

inline constexpr ker_addr_t phys_to_ker(phys_addr_t addr){
    return (addr|0xffff800000000000);
}

#endif
