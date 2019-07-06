#include <raw_ostream.h>
#include <memory/address.h>

#ifndef _VGAOUT_H_
#define _VGAOUT_H_
//VGA-compatible text mode
namespace text
{
enum ForeGroundColor
{
    ForeDarkBlack = 0,
    ForeDarkBlue,
    ForeDarkGreen,
    ForeDarkCyan,
    ForeDarkRed,
    ForeDarkMagenta,
    ForeDarkYellow,
    ForeGray,

    ForeBlack = 8,
    ForeBlue,
    ForeGreen,
    ForeCyan,
    ForeRed,
    ForeMagenta,
    ForeBrown,
    ForeWhite,
};

enum BackgroundColor
{
    BackBlack = 0,
    BackBlue,
    BackGreen,
    BackCyan,
    BackRed,
    BackMagenta,
    BackBrown,
    BackWhite,

    BackLightBlack = 8,
    BackLightBlue,
    BackLightGreen,
    BackLightCyan,
    BackLightRed,
    BackLightMagenta,
    BackLightBrown,
    BackLightWhite,
};
#pragma pack(push, 1)
struct color
{
    ForeGroundColor foreground : 4;
    BackgroundColor background : 4;
};
struct charactor
{
    char ansi;
    color attribute;
};
#pragma pack(pop)

inline static charactor * raw_screen() {return reinterpret_cast<charactor *>(phys_to_ker(0xb8000));}
void raw_print(int x, int y, const char *str);
void vga_initialize(int width,int height);
void vga_putchar(char ch);
void vga_puts(const char *str);
void vga_write(const char * str,size_t size);
void vga_linefeed();
void vga_tap();
void vga_clear();
void vga_backspace();
void vga_setCurrentColor(color c);
color vga_getCurrentColor();
void vga_updateScreenWithColor(color c);
} // namespace text

class vga_ostream : public text::raw_ostream{
public:
explicit vga_ostream():text::raw_ostream(){	
    text::vga_initialize(0,19);
}
protected:
virtual void write_impl(const char *str, size_t size) override{
    text::vga_write(str,size);
}
};
#endif