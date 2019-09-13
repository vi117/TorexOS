#pragma once
#include <stdint.h>
#include <drivers/driver.h>
#include <datastruc/list.h>
#include <memory/mm.h>

namespace x86_64
{
    struct irq_descriptor
    {
        util::list<drv::IrqHandler *> actions;
    };
    
    void IRQ_init();
    void register_handler(uint8_t irq, drv::IrqHandler * action);
    
} // namespace x86_64
