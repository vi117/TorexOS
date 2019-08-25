#pragma once
#include<memory/pageframe.h>
#include <datastruc/list.h>

namespace memory
{

inline PageDescriptor * buddyof(PageDescriptor * ptr,order_t order){
    uint64_t index = ptr - pd_base;
    index ^= 1 << order;
    return index + pd_base;
}
inline PageDescriptor * get_primary(PageDescriptor * a,PageDescriptor * b){
    uint64_t a_i = a - pd_base;
    uint64_t b_i = b - pd_base;
    return (a_i & b_i) + pd_base;
}

class BuddyAllocator
{
public:
    enum
    {
        MaxOrder = 9,
    };
    
    //index require 0~9
    FreeBlock *allocateWithOrder(order_t order,PageType pt);
    FreeBlock *splitBlock(FreeBlock *b);
    void tryMergeAndGet(FreeBlock * b);
    //it's not checking that argument is null.
    void deallocateBlock(PageDescriptor *b);

    util::list_base<FreeBlock, FreeBlock> freelist[MaxOrder+1];
    unsigned count[MaxOrder+1] = {0,};
};  
} // namespace memory
