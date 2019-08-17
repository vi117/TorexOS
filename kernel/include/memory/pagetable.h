#pragma once
#include<stdint.h>
#include<stddef.h>
#include<memory/address.h>
#include<arch/pagetable.h>

namespace memory
{
    //globla
    struct pgd_t;
    //
    struct pud_t;
    struct pmd_t;
    struct pte_t;
    enum{

    };
    
    void load_ptd(pgd_t *);

    void flush_tlb_all();
    void flush_tlb_single(void *);

} // namespace memory
