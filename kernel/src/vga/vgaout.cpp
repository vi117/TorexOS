#include <vga/vgaout.h>
#include <vga/cursor.h>

static text::Cursor cursor;
static text::charactor * screen;
static text::color literColor = {text::ForeWhite,text::BackMagenta};

void text::raw_print(int x,int y, const char * str){
    charactor * scr = text::raw_screen();
	scr += y * 80 + x;
	for (int i = 0; str[i] != 0; ++i)
	{
		scr->ansi = str[i];
		scr->attribute = { text::ForeGreen, text::BackBlack };
		scr++;
	}
}

void text::vga_initialize(int width,int height){
	vga_updateScreenWithColor(literColor);
    cursor.set(width,height);
	screen = text::raw_screen() + video_width*height+width;
}
void text::vga_putchar(char ch){
    screen->ansi = ch;
	screen->attribute = literColor;
	screen++;
	if (cursor.get() >= video_width * video_height - 1){
		vga_linefeed();
	}
	else{
		screen->attribute = literColor;
		cursor++;
	}
}
void text::vga_write(const char * str,size_t size){
	for(;size > 0; size--) {
        switch (*str)
		{
			case '\n':
				vga_linefeed();
				break;
			case '\t':
				vga_tap();
				break;
			case '\b':
				vga_backspace();
				break;
			default:
				vga_putchar(*str);
				break;
		}
        str++;
    }
}
void text::vga_puts(const char * str){
    while(*str != '\0') {
        switch (*str)
		{
			case '\n':
				vga_linefeed();
				break;
			case '\t':
				vga_tap();
				break;
			case '\b':
				vga_backspace();
				break;
			default:
				vga_putchar(*str);
				break;
		}
        str++;
    }
}
void text::vga_linefeed(){
    int y = cursor.gety();
	if (y == video_height - 1)
	{
		charactor * s = raw_screen();
		for (; s < raw_screen() + (video_height - 1)*video_width; s++)
			s[0] = s[video_width];
		for (; s < raw_screen() + video_width*video_height; s++){
			s->ansi = 0;
			s->attribute = literColor;
		}
		y--;
	}
	y++;
	cursor.set(0, y);
	screen = raw_screen() + y*video_width;
	screen->attribute = literColor;
}

void text::vga_backspace(){
    cursor = cursor.get() - 1;
	*(uint8_t *)(--screen) = 0;
}

void text::vga_tap(){
    int x = cursor.getx(), y = cursor.gety();
	x >>= 3;
	x++;
	x <<= 3;
	if (x >= video_width)
	{
		vga_linefeed();
		x = 0;
	}
	cursor.set(x, y);
	screen = raw_screen() + y*video_width + x;
}

void text::vga_clear(){
    for (screen = raw_screen();
		screen < raw_screen() + video_width * video_height; screen++)
		(*(uint16_t *)screen) = 0;
	cursor = 0;
}
text::color text::vga_getCurrentColor(){
	return literColor;
}
void text::vga_setCurrentColor(color c){
	literColor = c;
}
void text::vga_updateScreenWithColor(color c){
	auto scr = raw_screen();
	for(size_t i = 0; i < video_width * video_height; i++){
		scr[i].attribute = c;
	}
}