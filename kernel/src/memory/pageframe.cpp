#include <memory/pageframe.h>

namespace memory
{
PageDescriptor *pd_base;
}
void memory::mark_pages(PageDescriptor * pos,PageType type,order_t order){
    pos->flags(type);
    pos->orders(order);
    for (size_t i = 1; i < (1<<order); i++)
    {
        pos[i].flags(pt_tails);
    }
}

using namespace memory;


FreeBlock *BuddyAllocator::allocateWithOrder(order_t order, PageType pt)
{
    FreeBlock *ptr;
    if (order > MaxOrder)
        return nullptr;
    if (freelist[order].empty())
    {
        auto big = allocateWithOrder(order + 1, pt_free);
        if (big == nullptr)
            return nullptr;
        ptr = splitBlock(big);
    }
    else
    {
        ptr = freelist[order].front_iterator().get();
        freelist[order].pop_front_node();
        count[order]--;
    }
    ptr->flag = pt;
    ptr->size = order;
    return ptr;
}
FreeBlock *BuddyAllocator::splitBlock(FreeBlock *b)
{
    auto order = b->size;
    auto buddy = buddyof((PageDescriptor *)b, order - 1);
    buddy->orders(order - 1);
    buddy->flags(PageType::pt_free);
    freelist[order - 1].push_back_node(&buddy->free_block);
    count[order - 1]++;
    return b;
}
void BuddyAllocator::tryMergeAndGet(FreeBlock *b)
{
    auto order = b->size;
    if (order < MaxOrder)
    {
        auto buddy = buddyof((PageDescriptor *)b, order);
        if (buddy->flags() == pt_free && buddy->orders() == order)
        {
            freelist[order].pop_node((FreeBlock *)buddy);
            auto big = get_primary((PageDescriptor *)b, buddy);
            auto bigbuddy = buddyof(big, order);
            bigbuddy->flags(pt_tails);
            big->orders(order + 1);
            count[order] -= 2;
            count[order]++;
            return deallocateBlock(big);
        }
    }
    freelist[order].push_front_node(b);
    count[order]++;
}
void BuddyAllocator::deallocateBlock(PageDescriptor *b)
{
    if (b == nullptr)
        return;
    /*if(b->flags()==pt_free){
            double delete? we need more flag.
        }*/
    b->flags(pt_free);
    return tryMergeAndGet((FreeBlock *)b);
}