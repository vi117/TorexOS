#include <stdint.h>
#include <keyboard/scancode.hpp>
#include <drivers/driver.h>
#include <datastruc/CircularQueue.h>
#include <util/interrupt_lock.h>

#pragma once

namespace ps2
{
//keyboard LED 상태 설명 공용체.
//딱히 특별한 거 없음.
union KeyboardLED {
    struct
    {
        bool ScrollLock : 1;
        bool NumLock : 1;
        bool CapsLock : 1;
        bool changed : 1;
        bool padding : 4;
    };
    //...................................key pressed;
    uint8_t data;

    operator uint8_t()
    {
        return data;
    }
};
    // 
    // Return Value
    //true	: Success
    //false	: Failure
    bool activate();

    /* 키보드 LED 바꾸는 거.
			Return Value
			true	: Success
			false	: Failure */
    bool changeKeyboardLED(KeyboardLED state);

    bool isAviliable();
    key::ScanCode getScanCode();

    class Keyboard;
    class KeyboardIRQHandler : public drv::IrqHandler
    {
    public:
        KeyboardIRQHandler(Keyboard * ptr):context(ptr){}

        virtual drv::IrqStatus topHalf() override;
        
        Keyboard * context;
    };
    class Keyboard : public drv::CharactorDriver {
    public:
        Keyboard():irq_handler(this),initialized(false){}
        bool initialize();
        virtual bool probe() override {return initialize();}
        virtual const char * name() override{return "ps2";}
        virtual int version() override {return 1;}
        virtual size_t read(uint8_t * buf, size_t length) override;
        friend class KeyboardIRQHandler;
    private:
        util::CircularQueue<uint8_t,96> keyQueue;
        interrupt_lock keyboardQueueLock;
        KeyboardLED led;
        KeyboardIRQHandler irq_handler;
        bool initialized;
    };
} // namespace ps2
