#include <memory/pageframe.h>

namespace memory
{
PageDescriptor *pd_base;
}
void memory::mark_pages(PageDescriptor * pos,PageType type,order_t order){
    pos->flags(type);
    pos->orders(order);
    for (size_t i = 1; i < (static_cast<size_t>(1)<<order); i++)
    {
        pos[i].flags(pt_tails);
    }
}