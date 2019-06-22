#include<cursor.h>
#include<intrins.h>

enum VGA
{
	addressPort = 0x3d4,
	dataPort = 0x3d5,
	indexUpper=0x0e,
	indexLower=0x0f,
};

text::Cursor & text::Cursor::operator++()
{
	++x;
	if (x > video_width)
	{
		x = 0;
		x++;
	}
	update(); 
	return *this;
}

void text::Cursor::update()
{
	outbyte(addressPort, indexUpper);
	outbyte(dataPort, get() >> 8);
	outbyte(addressPort, indexLower);
	outbyte(dataPort, get() & 0xFF);
}