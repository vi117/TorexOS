#include<util/interrupt_lock.h>
#include<keyboard/scancode.hpp>
#include<drivers/ps2/keyboard.h>
#include<drivers/ps2/ps2.h>

using namespace ps2;

inline bool isOutBufFull()
{
	return (get_state_register().outbuf);
}
inline bool isInputBufFull()
{
	return (get_state_register().inbuf);
}
inline void wait_inputBuf()
{
	for (size_t i = 0; i < 0xFFFF; i++)
	{
		if (!isInputBufFull()) break;
	}
}
inline void wait_outputBuf()
{
	for (size_t i = 0; i < 0xFFFF; i++)
	{
		if (isOutBufFull()) break;
	}
}

bool ps2::state::activate()
{
	interrupt_lock lock;
	lock.lock();
	control_register.send(Control_Register::command::device_keyboard_active);
	wait_inputBuf();
	buffer.write(Buffer::command::device_keyboard_active);
	for (int i = 0; i < 100; i++)	//활성화 이전에 키 데이터가 들어 있을수 있으므로 100개 까지 조회 해본다.
	{
		wait_outputBuf();
		if (buffer.read() == Buffer::ack)
		{
			lock.unlock();
			return true;
		}
	}
	lock.unlock();
	return false;
}


bool ps2::state::changeKeyboardLED(ps2::KeyboardLED state)
{
	wait_outputBuf();
	buffer.write(Buffer::command::changeLED);
	wait_inputBuf();
	for (int i = 0; i < 100; i++)
	{
		wait_outputBuf();
		if (buffer.read() == Buffer::ack)
			goto Success;
	}
	return false;
Success:
	buffer.write(state.data);
	wait_inputBuf();
	for (int i = 0; i < 100; i++)
	{
		wait_outputBuf();
		if (buffer.read() == Buffer::ack)
			return true;
	}
	return false;
}

bool ps2::state::isAviliable()
{
	return isOutBufFull();
}

key::ScanCode ps2::state::getScanCode()
{	
	return buffer.read();
}