#pragma once
#include<stdint.h>
#include<memory/address.h>
#include <math/ilog2.h>

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
    pt_slab = 4,
};

typedef uint8_t PageFlag;
typedef uint8_t order_t;
struct CommonBlockInfo
{
    PageFlag flag;
    order_t size;
};

struct FreeBlock
{
    enum
    {
        null_index = 0xffffffffff
    };
    PageFlag flag;
    order_t size;
    FreeBlock * prev;
    FreeBlock * next;
    //page real size = 2^size
    typedef FreeBlock elem;
    inline FreeBlock *getNext() const {return next;}
    inline FreeBlock *getPrev() const {return prev;}
    inline void setNext(FreeBlock * ptr){next = ptr;}
    inline void setPrev(FreeBlock * ptr){prev = ptr;}
    constexpr static inline FreeBlock *getNullNode() {return nullptr;}
    inline FreeBlock *getDataPtr() { return (this); }
};

typedef uint32_t slubcell_t;
constexpr uint16_t in_use_max = 0xffff;
class slub_allocator;
struct SlubBlock
{
    PageFlag flag;
    order_t size;
    uint16_t in_use;
    slubcell_t free_offset;

    SlubBlock * next_block;
    SlubBlock * prev_block;
    slub_allocator * owner;

    inline SlubBlock *getNext() const {return next_block;}
    inline SlubBlock *getPrev() const {return prev_block;}
    inline void setNext(SlubBlock * ptr){next_block = ptr;}
    inline void setPrev(SlubBlock * ptr){prev_block = ptr;}
    constexpr static inline SlubBlock *getNullNode() {return nullptr;}
    inline SlubBlock *getDataPtr() { return (this); }
};

constexpr size_t smallest_page_size = 4_KB;

inline constexpr order_t calculOrder(size_t size){
    return util::math::ilog2(size) - util::math::ilog2(smallest_page_size);
}

//always 32 bytes...
union PageDescriptor{
    CommonBlockInfo common;
    FreeBlock free_block;
    SlubBlock slub_head;
    uint8_t padding[32];


    void flags(PageFlag pf){common.flag = pf;}
    PageFlag flags() const { return common.flag;}
    void orders(order_t o){common.size = o;}
    order_t orders() const {return common.size;}
};
static_assert(sizeof(PageDescriptor) == 32);
#pragma pack(pop)
inline ker_addr_t to_ker_addr(PageDescriptor * pd)
{
    return phys_addr_t{(pd - pd_base)*4_KB}.to_ker();
}

void mark_pages(PageDescriptor * pos,PageType type,order_t order);

} // namespace memory