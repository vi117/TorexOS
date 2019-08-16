#include <stdint.h>
#include <keyboard/scancode.hpp>

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

class state
{
public:
    // 키보드 활성화 함수, 대체적으로 바이오스에서 이미 활성화 시키지만 혹시 몰라서 또 시킴.
    // Return Value
    //true	: Success
    //false	: Failure
    bool activate();

    /* 키보드 LED 바꾸는 거.
			Return Value
			true	: Success
			false	: Failure */
    static bool changeKeyboardLED(KeyboardLED state);

    bool isAviliable();
    key::ScanCode getScanCode();
};
} // namespace ps2
