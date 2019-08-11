#pragma once
#include<stdint.h>
#include <datastruc/list.h>

namespace memory
{
union PageDescriptor;
extern PageDescriptor *pd_base;

#pragma pack(push, 1)

enum PageType{
    pt_free = 0,
    pt_kernel = 1,
    pt_tails = 2,
    pt_reserved = 3,
};  

typedef uint8_t PageFlag;
typedef uint8_t order_t;

struct CommonBlockInfo
{
    PageFlag flag;
    order_t size;
};

struct FreeBlock : public CommonBlockInfo
{
    enum
    {
        null_index = 0xffffffffff
    };
    uint64_t prev_index : 40;
    uint64_t next_index : 40;
    //page real size = 2^size
    typedef FreeBlock elem;
    inline FreeBlock *getNext() const;
    inline FreeBlock *getPrev() const;
    inline void setNext(FreeBlock *);
    inline void setPrev(FreeBlock *);
    static inline FreeBlock *getNullNode();
    inline FreeBlock *getDataPtr() { return (this); }
};

union PageDescriptor{
    CommonBlockInfo common;
    FreeBlock free_block;
    uint8_t padding[16];

    void flags(PageFlag pf){common.flag = pf;}
    PageFlag flags() const { return common.flag;}
    void orders(order_t o){common.size = o;}
    order_t orders() const {return common.size;}
};
//static_assert(sizeof(PageDescriptor) == 16);
#pragma pack(pop)
inline FreeBlock *FreeBlock::getNext() const{
    return (FreeBlock *)(next_index + pd_base);
}
inline FreeBlock *FreeBlock::getPrev() const { 
    return (FreeBlock *)(prev_index + pd_base); 
}
inline void FreeBlock::setNext(FreeBlock *ptr){
    next_index = ((PageDescriptor *)ptr) - pd_base;
}
inline void FreeBlock::setPrev(FreeBlock *ptr){
    prev_index = ((PageDescriptor *)ptr) - pd_base;
}
inline FreeBlock * FreeBlock::getNullNode(){
    return (FreeBlock *)(pd_base + null_index);
}

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
    void deallocateBlock(PageDescriptor *b);

    util::list::list_base<FreeBlock, FreeBlock> freelist[MaxOrder+1];
    unsigned count[MaxOrder+1] = {0,};
};

} // namespace memory