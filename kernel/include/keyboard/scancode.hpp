#pragma once
#include <stdint.h>

namespace key
{
enum : unsigned char
{
    NONE = 0x00,
    ENTER = '\n',
    TAB = '\t',
    ESC = 0x1B,
    BACKSPACE = 0x08,

    CTRL = 0x81,
    LSHIFT = 0x82,
    RSHIFT = 0x83,
    PRINTSCREEN = 0x84,
    LALT = 0x85,
    CAPSLOCK = 0x86,
    F1 = 0x87,
    F2 = 0x88,
    F3 = 0x89,
    F4 = 0x8A,
    F5 = 0x8B,
    F6 = 0x8C,
    F7 = 0x8D,
    F8 = 0x8E,
    F9 = 0x8F,
    F10 = 0x90,
    NUMLOCK = 0x91,
    SCROLLLOCK = 0x92,
    HOME    =0x93,
    UP      = 0x94,
    PAGEUP  = 0x95,
    LEFT    = 0x96,
    CENTER  = 0x97,
    RIGHT   = 0x98,
    END     = 0x99,
    DOWN    = 0x9A,
    PAGEDOWN = 0x9B,
    INS     = 0x9C,
    DEL     = 0x9D,
    F11     = 0x9E,
    F12     = 0x9F,
    PAUSE   = 0xA0,
    MOREWAIT = 0xFF,
};
union ScanCode {
    struct Code
    {
        uint8_t name : 7;
        bool up : 1;
    };
    Code code;
    uint8_t data;

    ScanCode() {}
    ScanCode(uint8_t t) : data(t) {}
    operator uint8_t() { return data; }
};

struct KeyMappingEntry
{
    // Shift 키나 Caps Lock 키와 조합되지 않는 ASCII 코드
    uint8_t NormalCode;

    // Shift 키나 Caps Lock 키와 조합된 ASCII 코드
    uint8_t CombinedCode;
};

// 스캔 코드 매핑 테이블에 대한 매크로
static constexpr int MappingTableMax = 89;

extern const KeyMappingEntry KeyMappingTable[MappingTableMax];

} // namespace key