#include <memory/mm.h>
#include <memory/memscan.h>
#include <algo/sort.h>
#include <datastruc/forward_list.h>
#include <memory/pageframe.h>
#include <datastruc/delay_constructor.h>

extern "C" char IST_END;

static util::delay_constructor<memory::BuddyAllocator> allocator;
static unsigned TotalMemoryBlockCount = 0;

//return : total memory bytes
static size_t caculatePageDescriptorSize(memory::e820MmapEntry * begin, memory::e820MmapEntry * end)
{
    using namespace memory;
    do
    {
        end--;
        if(end->type == e820MmapEntry::RegionType::Free){
            return (end->base + end->length) - phys_addr_t{nullptr};
        }
    }
    while(begin != end);
    return 0;
}
inline static ker_addr_t getPageDescriptorStart(){
    return ker_addr_t(&IST_END).align(sizeof(memory::PageDescriptor));
}
static phys_addr_t getFreeMemoryStart(size_t needBytesNum)
{
    phys_addr_t start{getPageDescriptorStart().to_phys()};
    const auto ret = start + (needBytesNum/4_KB)*sizeof(memory::PageDescriptor);
    return ret;
}
#include<debug/debug.h>
//Todo : replace magic number.
void memory::init()
{
    //sorting mm info.
    auto begin = e820_mmap_entry();
    auto end = begin + e820_mmap_entry_count();
    util::insertion_sort(begin, end, [](const e820MmapEntry &a, const e820MmapEntry &b) {
        return a.base < b.base;
    });
    auto cur_pd = getPageDescriptorStart().to_ptr_of<PageDescriptor>();
    pd_base = cur_pd;
    allocator.initialize();

    auto need_size = caculatePageDescriptorSize(begin,end);
    //get physical memory
    //2MB 조사해서 하나 하나 회수하기 귀찮음. 그래서 그냥 align.
    phys_addr_t start = getFreeMemoryStart(need_size).align(2_MB/*greatest page size... (we do not use 1GB page)*/);
    auto kernel_size = (start.to_ker() - (phys_addr_t{nullptr}).to_ker());
    for (size_t i = 0 ;i < kernel_size / 4_KB/*smallest page size*/; i++)
    {
        if(i % (1 << BuddyAllocator::MaxOrder) == 0){
            cur_pd->flags(pt_kernel);
            cur_pd->orders(BuddyAllocator::MaxOrder);
        }
        else
        {
            cur_pd->flags(pt_tails);
        }    
    }
    debug << "kernel space :" << start.address / 1_MB << " MB.\n";
    TotalMemoryBlockCount += start.address / 2_MB;
    while(begin != end && !begin->range_in(start,start)) begin++;
    if(begin == end) panic("corrupt e820 map!");
    debug << "memory state : ";
    for (;;)
    {
        if (begin->range_in(start, start + 2_MB)){
            if (begin->type == e820MmapEntry::RegionType::Free)
            {
                debug << "F";
                cur_pd->flags(pt_free);
                cur_pd->orders(BuddyAllocator::MaxOrder);
                allocator.value.freelist[BuddyAllocator::MaxOrder].push_back_node((FreeBlock *)cur_pd);
                allocator.value.count[BuddyAllocator::MaxOrder]++;
            }
            else{
                debug << "R";
                cur_pd->flags(pt_reserved);
                cur_pd->orders(BuddyAllocator::MaxOrder);
            }
            for (size_t i = 1; i < (1 << BuddyAllocator::MaxOrder); i++)
            {
                cur_pd[i].flags(pt_tails);
            }
            cur_pd += 1 << BuddyAllocator::MaxOrder; //512.
            start += 2_MB;
            TotalMemoryBlockCount++;
        }
        else{
            debug << "R";
            cur_pd->flags(pt_reserved);
            cur_pd->orders(BuddyAllocator::MaxOrder);
            for (size_t i = 1; i < (1 << BuddyAllocator::MaxOrder); i++)
            {
                cur_pd[i].flags(pt_tails);
            }
            cur_pd += 1 << BuddyAllocator::MaxOrder;
            start += 2_MB;
            TotalMemoryBlockCount++;
            while(!(begin == end || begin->range_in(start,start) )) begin++;
            if(begin == end) break;
        }
    }
    debug << "\n";
}
unsigned memory::getFreeMemoryBlockCount(){
    return allocator.value.count[BuddyAllocator::MaxOrder];
}
unsigned memory::getTotalMemoryBlockCount(){
    return TotalMemoryBlockCount;
}
void memory::printMemoryCount(text::raw_ostream & os){
    os << "Total Memory : " << TotalMemoryBlockCount*2 << "MB\n"
    "Current Free Memory : "<< getFreeMemoryBlockCount()*2 << "MB\n";
    //Todo : hard coding. fix later
}

/*
static bool kernel_mapped[64] ;
bool memory::is_mapped_in_kernel(phys_addr_t addr){
    return kernel_mapped[(addr.align(1_GB).address / 1_GB - 1)];
}*/