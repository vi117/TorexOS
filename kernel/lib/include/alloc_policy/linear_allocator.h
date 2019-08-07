#include <stddef.h>

#ifndef _LINEAR_ALLOCATOR_H_
#define _LINEAR_ALLOCATOR_H_

namespace util
{
    //not thread safe.
    class LinearAllocator{
    public:
        explicit LinearAllocator(uint8_t * start_ptr ,uint8_t * end_ptr )
            :addr(start_ptr),end(end_ptr){}
        inline void * allocate_bytes(size_t n){
            auto ret = addr;
            addr += n;
            if(addr >= end){
                addr = ret;
                return nullptr;
            }
            return ret;
        }
        void deallocate_bytes(uint8_t *){/*do nothing.*/}
    private:
        uint8_t * addr;
        uint8_t * end;
    };
} // util


#endif