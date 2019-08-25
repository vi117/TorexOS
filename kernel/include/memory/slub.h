#pragma once
#include<stddef.h>
#include<memory/pageframe.h>
#include<datastruc/list.h>

namespace memory
{
static constexpr slubcell_t cell_null = 0xffffffff;

void * alloc_in_oneblock(SlubBlock *);
//ptr must be already checked that it's in range of block.
void free_in_oneblock(SlubBlock * block,void * ptr);

class SlubAllocator{
public:
    void * alloc();
    //assure this has a argument ptr.
    void free(SlubBlock * block,void * ptr);
    
    void make_slabs(SlubBlock *);

    util::list_base<SlubBlock,SlubBlock> partial;
    util::list_base<SlubBlock,SlubBlock> full;
    uint16_t object_size;
};

}