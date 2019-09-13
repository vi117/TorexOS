#pragma once
#include<datastruc/com_string.h>

namespace drv
{
    class IDriver
    {
    public:
    virtual const char * name() = 0;
    virtual bool probe() { return false; }
    virtual int version() = 0;
    virtual ~IDriver() noexcept{}
    };

    class CharactorDriver : public IDriver
    {
    public:
        virtual size_t read([[maybe_unused]] uint8_t * buf,[[maybe_unused]] size_t length){return 0;}
        virtual size_t write([[maybe_unused]] uint8_t * buf,[[maybe_unused]] size_t length){return 0;}
    };

    enum class IrqStatus
    {
        Success = 0,
        NotHandled = 1,
    };
    class IrqHandler
    {
    public:
        virtual IrqStatus topHalf()
        {
            return IrqStatus::NotHandled;
        }
        virtual ~IrqHandler() noexcept {}

        virtual const char * name(){return "NeverCalled";}
    };
} // namespace drv

namespace driver = drv;