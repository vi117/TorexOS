#include <memory/mm.h>
#include <memory/memscan.h>
#include <memory/buddy.h>
#include <memory/slub.h>

#include <algo/sort.h>

#include <math/ilog2.h>
#include <debug/logger.h>
#include <arch/pagetable.h>

//Todo : replace magic number. 2_MB.

extern "C" char IST_END;

static memory::BuddyAllocator allocator;
static unsigned TotalMemoryBlockCount = 0;
//32, 64, 128, 256, 512, 1024, 2048.
static memory::SlubAllocator slubs[7];

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
    const auto ret = start + (needBytesNum / memory::smallest_page_size) * sizeof(memory::PageDescriptor);
    return ret;
}
// return value : pointer of end of kernel area
static phys_addr_t mark_kernel_area()
{
    using namespace memory;
    auto pd_start = pd_base;
    const auto required_size = calculatePageDescriptorSize();
    //get physical memory
    //It's annoying to probe kernel memory area in detail. so I just align by 2MB.
    phys_addr_t start = getFreeMemoryStart(required_size).align(2_MB /*greatest page size... (we do not use 1GB page)*/);
    auto kernel_size = (start.to_ker() - (phys_addr_t{nullptr}).to_ker());
    for (size_t i = 0; i < kernel_size / 1_MB /*greatest page size / 2*/; i++)
        mark_pages(&pd_start[i], pt_kernel, BuddyAllocator::MaxOrder-1 /*ilog2(1_MB/4_KB)*/);
    
    TotalMemoryBlockCount += kernel_size / 2_MB;
    logger << "Kernel Space : " << kernel_size / 1_MB << " MB\n";
    return start;
}

static void makeBuddy(phys_addr_t start){
    using namespace memory;
    
    const e820MmapEntry * begin = e820_mmap_entry();
    const e820MmapEntry * end = begin + e820_mmap_entry_count();
    auto cur_pd = pd_base + ((start - nullptr) / smallest_page_size);
    //allocator.initialize();
    
    while (begin != end && !begin->range_in(start, start))
        begin++;
    if (begin == end)
        panic("corrupt e820 map!");
    for (;;)
    {
        if (begin->range_in(start, start + 2_MB))
        {
            if (begin->type == e820MmapEntry::RegionType::Free)
            {
                mark_pages(cur_pd, pt_free, BuddyAllocator::MaxOrder);
                allocator.freelist[BuddyAllocator::MaxOrder].push_back_node((FreeBlock *)cur_pd);
                allocator.count[BuddyAllocator::MaxOrder]++;
            }
            else
            {
                mark_pages(cur_pd, pt_reserved, BuddyAllocator::MaxOrder);
            }
            cur_pd += 1 << BuddyAllocator::MaxOrder; //512.
            start += 2_MB;
            TotalMemoryBlockCount++;
        }
        else
        {
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
}
static void makePool()
{
    const int sizes[] = {32, 64, 128, 256, 512, 1024, 2048};
    for (size_t i = 0; i < 7; i++)
    {
        slubs[i].object_size = sizes[i];
    }
}
static size_t size_of_mapping(){
    const mm::e820MmapEntry * begin = mm::e820_mmap_entry();
    const mm::e820MmapEntry * end = begin + mm::e820_mmap_entry_count();
    auto max = util::max_element(begin,end,
    [](const mm::e820MmapEntry & a,const mm::e820MmapEntry & b){
        return a.base + a.length < b.base + b.length;
    });
    return max->base.address + max->length - 1;
}
static void makeKernelPTE()
{
    phys_addr_t end_position{size_of_mapping()};
    const auto pte_count = (end_position.address / 2_MB) / 512 + 1;
    const auto pud_count = (pte_count - 1) / 512 + 1;
    const auto pgd_count = (pud_count - 1) / 512 + 1;
    if(pte_count>512){panic("we not support memory above 512GB.");}
    //if(pgd_count>1){panic("memory is too big to handle!!!");}
    auto pte_base = mm::alloc_pages_kma(mm::pt_kernel_map,pte_count*4_KB);
    auto pud_base = mm::alloc_pages_kma(mm::pt_kernel_map,pud_count*4_KB);
    auto pgd_base = mm::alloc_pages_kma(mm::pt_kernel_map,pgd_count*4_KB);

    auto pte = pte_base.to_ptr_of<x86_64::PageDirectoryEntry2MB>();
    auto pud = pud_base.to_ptr_of<x86_64::PageDirectoryPointerEntry>();
    auto pgd = pgd_base.to_ptr_of<x86_64::PageMapLevel4>();
    /*debug << (void *)pte <<" : " << pte_count <<"\n";
    debug << (void *)pud <<" : " << pud_count <<"\n";
    debug << (void *)pgd <<" : " << pgd_count <<"\n";*/

    for (auto addr = phys_addr_t{nullptr};
    addr < end_position; addr += 2_MB, pte++)
    {
        pte->pointTo(addr.address);
        pte->Present = true;
        pte->RW = true;
        pte->PS = true;
    }
    for ( size_t i = 0;i < pte_count;i++)
    {
        pud[i].pointTo(pte_base.address + i*4_KB);
        pud[i].Present = true;
        pud[i].RW = true;
    }
    /*In fact, currently we don't have to use 'for' statement,
     because currently pud_count equal to 1.
     But we use loop statement to prepare for the future.
     */
    for ( size_t i = 0;i < pud_count;i++)
    {
        pgd[i+256].pointTo(pud_base.address + i*4_KB);
        pgd[i+256].Present = true;
        pgd[i+256].RW = true;
    }
    //debug << (void *)(ker_addr_t{pgd}.to_phys().address) << "\n";
    x86_64::load_PML4(pgd);
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
    makeKernelPTE();
    makePool();
}
unsigned memory::getFreeMemoryBlockCount()
{
    return allocator.count[BuddyAllocator::MaxOrder];
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

memory::PageDescriptor * memory::alloc_pages(PageType pt,order_t order)
{
    return (PageDescriptor *)allocator.allocateWithOrder(order,pt);
}
void memory::free_pages(PageDescriptor * pd)
{
    if(pd == nullptr) return;
    allocator.deallocateBlock(pd);
}
ker_addr_t memory::alloc_pages_kma(PageType pt, size_t sz)
{
    return to_ker_addr(alloc_pages(pt,calculOrder(sz)));
}
void * memory::kmalloc(size_t size){
    if (size == 0) panic("alloc with 0 size");
    if (size > 2_MB) panic("Too big to alloc");
    if(size > 2048){
        return alloc_pages(pt_kernel,calculOrder(size));
    }
    size = util::max((size_t)32, size);
    const auto index = util::math::ilog2(size-1) - util::math::ilog2(32-1);
    return slubs[index].alloc();
}
void memory::kfree(void * ptr){
    bool allow_kernel = true;
    size_t index = (ker_addr_t{ptr}.to_phys().address >> util::math::ilog2(smallest_page_size));
    while(pd_base[index].flags() == pt_tails){
        allow_kernel = false;
        //turn off right bit set.
        index = (index & (index - 1));
    }
    auto pd = &pd_base[index];
        switch (pd->flags())
        {
    case pt_kernel:
        if(allow_kernel)
        {
            free_pages(pd);
        }
        else
        {
            panic("free through wrong allocator");
        }
        break;
    case pt_slab:
        pd->slub_head.owner->free(&pd->slub_head,ptr);
        break;
    case pt_free:
        panic("double free");
        break;
    default:
        panic("invalid pointer");
        break;
    }
    
}

void * operator new(size_t sz){
    return memory::kmalloc(sz);
}
void * operator new(size_t sz, [[maybe_unused]]const util::nothrow_t nothrow_value) noexcept{
    return memory::kmalloc(sz);
}
void* operator new[] (size_t sz)
{
return memory::kmalloc(sz);
}
void* operator new[] (size_t sz, [[maybe_unused]]const util::nothrow_t& nothrow_value) noexcept
{
return memory::kmalloc(sz);
}

void operator delete(void * ptr)
{
    memory::kfree(ptr);
}
void operator delete(void * ptr,[[maybe_unused]]size_t size){
    memory::kfree(ptr);
}
void operator delete[](void * ptr)
{
    memory::kfree(ptr);
}
void operator delete[](void * ptr,[[maybe_unused]]size_t size){
    memory::kfree(ptr);
}