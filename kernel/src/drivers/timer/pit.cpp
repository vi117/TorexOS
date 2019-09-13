#include <intrins.h>
#include <util/interrupt_lock.h>

#include <drivers/driver.h>
#include <drivers/timer/pit.h>
#include <process/scheduler.h>

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
class SchedPitTimerHandler : public drv::IrqHandler
{
public:
	virtual drv::IrqStatus topHalf() override
	{
		count++;
		if(expired <= count)
		{
			ps::current_scheduler().tick();
		}
		return drv::IrqStatus::Success;
	}
	virtual const char * name() override {return "PIT_Timer";}
	void set_expired(uint64_t c)
	{
		expired = count + c;
	}
	uint64_t count = 0;
	uint64_t expired = 0xffffffffffffffff;
};

#include<arch/interrupt.h>

class SchedPitTimer : public ps::SchedulerTimer
{
public:
	void init()
	{
		using namespace PIT;
		initialize(msToCount(1),true);
		x86_64::register_handler(0,&handler);
	}
	virtual void expired_after(util::millisecond_t ms) override
	{
		expired_count(ms.count());
	}
	virtual void expired_count(uint64_t c) override
	{
		handler.set_expired(c);
	}

	SchedPitTimerHandler handler;
};

static SchedPitTimer timer;
void PIT::init_for_sched(){timer.init();}
ps::SchedulerTimer * PIT::get_PIT()
{
	return &timer;
}