#include <stddef.h>

#ifndef _LINEAR_ALLOCATOR_H_
#define _LINEAR_ALLOCATOR_H_

namespace util
{
    template<typename Ty>
    class LinearAllocator{
    public:
        explicit LinearAllocator(Ty * start_ptr ,Ty * end_ptr )
            :addr(start_ptr),end(end_ptr){}
        Ty * allocate(size_t n){
            auto ret = addr;
            addr += n;
            if(addr >= end){
                addr = ret;
                return nullptr;
            }
            return ret;
        }
        void deallocate(Ty *){/*do nothing.*/}
    private:
        Ty * addr;
        Ty * end;
    };
} // util


#endif