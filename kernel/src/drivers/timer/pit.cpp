#include <drivers/timer/pit.h>
#include <intrins.h>
#include <util/interrupt_lock.h>

enum SelectCounter
{
	control_Counter0 = 0x00,
	control_Counter1 = 0x40,
	control_Counter2 = 0x80,
};
enum ReadWrite
{
	control_Latch = 0x00,
	control_LSBMSBRW = 0x30,
};
enum mode
{
	control_Mode0 = 0x00, //interrupt during counting : no interrupt. send interrupt only if counter 0.
	control_Mode1 = 0x02,
	control_Mode2 = 0x04, //Clock rate generator : interrrupt.
	control_Mode3 = 0x06,
	control_Mode4 = 0x08,
	control_Mode5 = 0x0A,
};
enum BCD
{
	format_BCD = 1,
	format_Binary = 0,
};

void PIT::initialize(uint16_t count, bool periodic)
{
	interrupt_lock t;
	t.lock();
	if (periodic)
		outbyte(port_Control, control_Counter0 | control_LSBMSBRW | control_Mode2 | format_Binary);
	else
		outbyte(port_Control, control_Counter0 | control_LSBMSBRW | control_Mode0 | format_Binary);
	outbyte(port_Counter0, (uint8_t)(count & 0x00FF));
	outbyte(port_Counter0, count >> 8);
	t.unlock();
}
uint16_t PIT::ReadCounter0()
{
	uint16_t high, low;
	interrupt_lock t;
	t.lock();
	outbyte(port_Control, control_Latch);
	low = inbyte(port_Counter0);
	high = inbyte(port_Counter0);
	t.unlock();
	high <<= 8;
	high |= low;
	return high;
}

void PIT::waitUsingDirectPIT(uint16_t count)
{
	interrupt_lock t;
	t.lock();
	initialize(0, false); //0x10000 setting.

	uint16_t current;
	uint16_t last = ReadCounter0();
	do
	{
		current = ReadCounter0();
	} while (((last - current) & 0xffff) < count);
	t.unlock();
}
void PIT::waitMs(unsigned long Millisecond)
{
	for (size_t i = 0; i < (Millisecond >> 5); i++)
		waitUsingDirectPIT(msToCount(0x20));
	waitUsingDirectPIT(msToCount(Millisecond & 0x1F));
}
