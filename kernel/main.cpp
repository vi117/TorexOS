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
#pragma pack(push,1)
typedef struct
{
    ForeGroundColor foreground : 4;
    BackgroundColor background : 4;
} color;
typedef struct
{
    char ansi;
    color attribute;
} charactor;
#pragma pack(pop)
void RawPrintString(int x, int y, const char *str)
{
    charactor *screen = (charactor *)0xb8000;
    screen += y * 80 + x;
    for (int i = 0; str[i] != 0; i++)
    {
        screen->ansi = str[i];
        screen->attribute.foreground = ForeCyan;
        screen->attribute.background = BackBlack;
        screen++;
    }
}

int main()
{
    RawPrintString(0,19,"Hello, World!");
    while(1){
        /* do nothing. */
    }
}