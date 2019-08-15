#include <memory/mm.h>
#include <memory/memscan.h>
#include <memory/buddy.h>

#include <algo/sort.h>

#include <datastruc/delay_constructor.h>
#include <math/ilog2.h>
#include <debug/debug.h>

//Todo : replace magic number.

extern "C" char IST_END;

static util::delay_constructor<memory::BuddyAllocator> allocator;
static unsigned TotalMemoryBlockCount = 0;

//return : total memory bytes
static size_t calculatePageDescriptorSize()
{
    using namespace memory;
    const e820MmapEntry * begin = e820_mmap_entry();
    const e820MmapEntry * end = begin + e820_mmap_entry_count();
    do
    {
        end--;
        if (end->type == e820MmapEntry::RegionType::Free)
        {
            return (end->base + end->length) - phys_addr_t{nullptr};
        }
    } while (begin != end);
    return 0;
}
inline static ker_addr_t getPageDescriptorStart()
{
    return ker_addr_t(&IST_END).align(sizeof(memory::PageDescriptor));
}
static phys_addr_t getFreeMemoryStart(size_t needBytesNum)
{
    phys_addr_t start{getPageDescriptorStart().to_phys()};
    const auto ret = start + (needBytesNum / 4_KB) * sizeof(memory::PageDescriptor);
    return ret;
}
// return value : pointer of end of kernel area
static phys_addr_t mark_kernel_area()
{
    using namespace memory;
    auto pd_start = pd_base;
    const auto required_size = calculatePageDescriptorSize();
    //get physical memory
    //2MB 조사해서 하나 하나 회수하기 귀찮음. 그래서 그냥 align.
    phys_addr_t start = getFreeMemoryStart(required_size).align(2_MB /*greatest page size... (we do not use 1GB page)*/);
    auto kernel_size = (start.to_ker() - (phys_addr_t{nullptr}).to_ker());
    for (size_t i = 0; i < kernel_size / 2_MB /*greatest page size*/; i++)
        mark_pages(&pd_start[i], pt_kernel, BuddyAllocator::MaxOrder /*ilog2(2_MB/4_KB)*/);
    
    TotalMemoryBlockCount += kernel_size / 2_MB;
    debug << "kernel space : " << kernel_size / 1_MB << " MB.\n";
    return start;
}

static void makeBuddy(phys_addr_t start){
    using namespace memory;
    
    const e820MmapEntry * begin = e820_mmap_entry();
    const e820MmapEntry * end = begin + e820_mmap_entry_count();
    allocator.initialize();
    
    while (begin != end && !begin->range_in(start, start))
        begin++;
    if (begin == end)
        panic("corrupt e820 map!");
    debug << "memory state : ";
    for (;;)
    {
        if (begin->range_in(start, start + 2_MB))
        {
            if (begin->type == e820MmapEntry::RegionType::Free)
            {
                debug << "F";
                mark_pages(cur_pd, pt_free, BuddyAllocator::MaxOrder);
                allocator.value.freelist[BuddyAllocator::MaxOrder].push_back_node((FreeBlock *)cur_pd);
                allocator.value.count[BuddyAllocator::MaxOrder]++;
            }
            else
            {
                debug << "R";
                mark_pages(cur_pd, pt_reserved, BuddyAllocator::MaxOrder);
            }
            cur_pd += 1 << BuddyAllocator::MaxOrder; //512.
            start += 2_MB;
            TotalMemoryBlockCount++;
        }
        else
        {
            debug << "R";
            mark_pages(cur_pd, pt_reserved, BuddyAllocator::MaxOrder);
            cur_pd += 1 << BuddyAllocator::MaxOrder;
            start += 2_MB;
            TotalMemoryBlockCount++;
            while (!(begin == end || begin->range_in(start, start)))
                begin++;
            if (begin == end)
                break;
        }
    }
    debug << "\n";
}
static void makePool()
{

}
void memory::init()
{
    //sorting mm info.
    auto begin = e820_mmap_entry();
    auto end = begin + e820_mmap_entry_count();
    util::insertion_sort(begin, end,
                         [](const e820MmapEntry &a, const e820MmapEntry &b) {
                             return a.base < b.base;
                         });

    //initialize pd base.
    pd_base = getPageDescriptorStart().to_ptr_of<PageDescriptor>();
    auto start = mark_kernel_area();
    
    makeBuddy(start);
    //makeKernelPTE();
    makePool();
}
unsigned memory::getFreeMemoryBlockCount()
{
    return allocator.value.count[BuddyAllocator::MaxOrder];
}
unsigned memory::getTotalMemoryBlockCount()
{
    return TotalMemoryBlockCount;
}
void memory::printMemoryCount(text::raw_ostream &os)
{
    os << "Total Memory : " << TotalMemoryBlockCount * 2 << "MB\n"
                                                            "Current Free : "
       << getFreeMemoryBlockCount() * 2 << "MB\n";
    //Todo : hard coding. fix later
}

/*
static bool kernel_mapped[64] ;
bool memory::is_mapped_in_kernel(phys_addr_t addr){
    return kernel_mapped[(addr.align(1_GB).address / 1_GB - 1)];
}*/

/*void * memory::kmalloc(size_t size){
    if (size == 0) //get error!
    ;
    if(size > 2048){
        //alloc pages...
    }
    if(size < 32) size = 32;
    auto index = util::math::ilog2(size) - util::math::ilog2(32);
    //return slub_allocators[index].allocate();
}*/