#pragma once
#include<datastruc/com_string.h>

namespace drv
{
    class IDriver
    {
    public:
    virtual const char * name() = 0;
    virtual bool probe() {return false;}
    virtual int version() = 0;
    };

    class CharactorDriver : public IDriver
    {
    public:
        virtual size_t read([[maybe_unused]] uint8_t * buf,[[maybe_unused]] size_t length){return 0;}
        virtual size_t write([[maybe_unused]] uint8_t * buf,[[maybe_unused]] size_t length){return 0;}
    };

    enum IrqStatus
    {
        Irq_Status_Success = 0,
    };
    class IrqHandler
    {
    public:
        virtual IrqStatus topHalf()
        {
            return Irq_Status_Success;
        }
    };
} // namespace drv

namespace driver = drv;