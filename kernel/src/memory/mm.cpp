#include <memory/mm.h>
#include <memory/memscan.h>
#include <algo/sort.h>
#include <alloc_policy/pool_allocator.h>
#include <datastruc/forward_list.h>

extern "C" char IST_END;
class customAllocator;

static phys_addr_t free_page_start_address(nullptr);
using free_huge_page_node = util::forward_list_default_node<phys_addr_t>;

class customAllocator : public util::PoolAllocator<free_huge_page_node>
{
public:
    using base_type = util::PoolAllocator<free_huge_page_node>;
    customAllocator();
    inline free_huge_page_node *allocate()
    {
        return base_type::allocate();
    }

private:
    void add_memblock(ker_addr_t start, size_t length);

private:
};

static util::forward_list<phys_addr_t, customAllocator> FreeHugePageList;

customAllocator::customAllocator()
{
    phys_addr_t start{ker_addr_t(&IST_END).to_phys()};
    //Huge page's size = 2MB.
    const auto aligned = start.align(2_MB);
    if (aligned - start > 0)
    {
        add_memblock(start.to_ker(), aligned - start);
        start = aligned;
    }
    else
    {
        add_memblock(start.to_ker(), 2_MB);
        start += 2_MB;
    }
    free_page_start_address = start;
}
void customAllocator::add_memblock(ker_addr_t start, size_t length)
{
    auto count = length / sizeof(free_huge_page_node);
    auto node_ptr = start.to_ptr_of<free_huge_page_node>();
    for (size_t i = 0; i < count; i++)
    {
        push(addressof(node_ptr[i]));
    }
}

void memory::init()
{
    //sorting mm info.
    auto begin = e820_mmap_entry();
    auto end = begin + e820_mmap_entry_count();
    util::insertion_sort(begin, end, [](const e820MmapEtnry &a, const e820MmapEtnry &b) {
        return a.base < b.base;
    });
    //get physical memory
    phys_addr_t start{free_page_start_address};
    while (begin != end)
    {
        if (begin->type == e820MmapEtnry::RegionType::Free && begin->range_in(start, start + 2_MB))
        {
            FreeHugePageList.push_front(start);
            start += 2_MB;
            continue;
        }
        begin++;
    }
}

phys_addr_t memory::alloc_huge_page()
{
    const auto ret = FreeHugePageList.front();
    FreeHugePageList.pop_front();
    return ret.value_or(nullptr);
}
void memory::free_huge_page(phys_addr_t addr)
{
    FreeHugePageList.push_front(addr);
}