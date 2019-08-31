#include<stdint.h>

namespace CMOS
{
inline auto BcdToBinary(uint8_t x) -> uint8_t
{
    return ((x >> 4) * 10) + (x & 0x0F);
}
#pragma pack(push,1)
struct CMOS_RTC
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t year;
    uint8_t month;
    uint8_t dayOfMonth;
    uint8_t dayOfWeek;
    
    void update();
    
    const char *dayOfWeekToCString();
};
#pragma pack(pop)
} // namespace CMOS