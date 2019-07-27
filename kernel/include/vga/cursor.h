#include <stdint.h>

#ifndef _Cursor_H_
#define _Cursor_H_

namespace text
{
constexpr int video_width = 80;
constexpr int video_height = 25;

inline uint16_t xyToOffset(int x, int y)
{
	return x + y * video_width;
}
inline void OffsetToxy(uint16_t offset, int &x, int &y)
{
	x = (offset % video_width);
	y = (offset / video_width);
}
class Cursor
{
  public:
	constexpr Cursor() : x(0), y(0) {}
	uint16_t get() const { return xyToOffset(x, y); };
	void set(uint16_t offset)
	{
		OffsetToxy(offset, x, y);
		update();
	}
	Cursor &operator=(uint16_t Offset)
	{
		set(Offset);
		return *this;
	};
	Cursor &operator++();
	Cursor &operator++(int) { return operator++(); }
	void set(int new_x, int new_y)
	{
		x = new_x;
		y = new_y;
		update();
	}
	int getx() { return x; }
	int gety() { return y; }

  private:
	void update();

  private:
	int x;
	int y;
};
} // namespace text

#endif