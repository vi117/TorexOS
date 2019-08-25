#include<drivers/timer/cmos.h>
#include<intrins.h>
#include<util/interrupt_lock.h>

enum RTC
{
	CMOSAddress = 0x70,
	CMOSData = 0x71,

	AddressSecond = 0x00,
	AddressMinute = 0x02,
	AddressHour = 0x04,
	AddressDayOfWeek = 0x06,
	AddressDayOfMonth = 0x07,
	AddressMonth= 0x08,
	AddressYear = 0x09,
};

void CMOS::CMOS_RTC::update()
{
	interrupt_lock t;
	t.lock();
	outbyte(CMOSAddress, AddressSecond);
	second = BcdToBinary(inbyte(CMOSData));
	outbyte(CMOSAddress, AddressMinute);
	minute = BcdToBinary(inbyte(CMOSData));
	outbyte(CMOSAddress, AddressHour);
	hour = BcdToBinary(inbyte(CMOSData));

	outbyte(CMOSAddress, AddressDayOfWeek);
	dayOfWeek = BcdToBinary(inbyte(CMOSData));
	outbyte(CMOSAddress, AddressDayOfMonth);
	dayOfMonth = BcdToBinary(inbyte(CMOSData));
	outbyte(CMOSAddress, AddressMonth);
	month = BcdToBinary(inbyte(CMOSData));
	outbyte(CMOSAddress, AddressYear);
	year = BcdToBinary(inbyte(CMOSData)) + 2000;
	t.unlock();
}

const char * CMOS::CMOS_RTC::dayOfWeekToCString()
{
	static const char * dayOfWeekStringTable[8] = { "Error", "Sunday","Monday",
		"Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	if (dayOfWeek >= 8)
		return dayOfWeekStringTable[0];
	return dayOfWeekStringTable[dayOfWeek];
}