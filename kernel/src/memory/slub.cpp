#include <memory/slub.h>
#include <memory/mm.h>
#include <debug/debug.h>


void * memory::alloc_in_oneblock(SlubBlock * ptr)
{
    if(ptr->free_offset == cell_null) return nullptr;
    auto base = to_ker_addr((PageDescriptor *)ptr).to_ptr_of<uint8_t>();
    auto object = reinterpret_cast<slubcell_t *>(&base[ptr->free_offset]);
    ptr->free_offset = *object;
    ptr->in_use++;
    return static_cast<void *>(object);
/*
meta 정보를 추가해야하는.
red_zone 이나 poison, owner_track은 나중에 구현.
#ifndef DEBUG
    auto base = to_ker_addr(ptr).to_ptr_of<uint8_t>();
    auto object = reinterpret_cast<uint32_t *>(&base[ptr->free_offset]);
    ptr->free_offset = *object;
    return static_cast<void *>(object);
#else

#endif
*/
}
void memory::free_in_oneblock(SlubBlock * block, void * ptr)
{
    auto base = to_ker_addr((PageDescriptor *)block).to_ptr_of<uint8_t>();
    *((slubcell_t *)ptr) = block->free_offset;
    block->free_offset = ((uint8_t *)ptr) - base;
    block->in_use--;
/*
meta info check 도 나중에함.
#ifndef DEBUG
    auto base = to_ker_addr((PageDescriptor *)block).to_ptr_of<uint8_t>();
    *((uint32_t *)ptr) = block->free_offset;
    block->free_offset = ((uint8_t *)ptr) - base;
#else

#endif
*/
}
void memory::SlubAllocator::make_slabs(SlubBlock * block)
{
    const auto base = to_ker_addr((PageDescriptor *)block).to_ptr_of<uint8_t>();
    const auto limit = ((size_t)smallest_page_size << block->size);
    block->owner = this;
    //debug << base << ":" << limit << "(" << object_size<<")";
    size_t index = 0;
    while(index < limit){
        *((slubcell_t *)(base+index))= index + object_size;
        index += object_size;
    }
    index -= object_size;
    *((slubcell_t *)(base+index))= cell_null;
}

void * memory::SlubAllocator::alloc(){
    if(!partial.empty()){
        auto& front = partial.front();
        if (front.free_offset != cell_null)
            return alloc_in_oneblock(&front);
        else{
            partial.pop_front_node();
            full.push_back_node(&front);
            front.in_use = in_use_max;
            return alloc();
        }
    }    
    //128 objects per 1 slab seems to be enough.
    const auto allocate_order = calculOrder(object_size*128);
    //debug << (size_t)allocate_order << "\n";

    auto pages = memory::alloc_pages(pt_slab,allocate_order);
    if(pages == nullptr) return nullptr;
    make_slabs((SlubBlock *)pages);
    partial.push_front_node((SlubBlock *)pages);
    return alloc();
}
void memory::SlubAllocator::free(SlubBlock * block,void * ptr){
    if(block->free_offset == cell_null)
    {
        free_in_oneblock(block,ptr);
        full.pop_node(block);
        partial.push_back_node(block);
    }
    else{
        free_in_oneblock(block,ptr); 
        if(block->in_use == 0)
        {
            partial.pop_node(block);
            free_pages((PageDescriptor *)block);
        }
    }
}