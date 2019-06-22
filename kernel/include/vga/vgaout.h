

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

inline static charactor * raw_screen() {return reinterpret_cast<charactor *>(0xb8000);}
void raw_print(int x, int y, const char *str);
void initialize();
void putchar(char ch);
void puts(const char *str);
void linefeed();
void tap();
void clear();
void backspace();
void puts(bool b);
} // namespace text

#endif