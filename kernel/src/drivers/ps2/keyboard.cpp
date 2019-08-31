#include <drivers/ps2/keyboard.h>
#include <arch/interrupt.h>
#include <util/interrupt_lock.h>
#include <thread/mutex.h>
#include <keyboard/scancode.hpp>
#include <drivers/ps2/ps2.h>
#include <debug/debug.h>

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
//we need refactorying....
bool ps2::activate()
{
	interrupt_lock lock;
	util::lock_guard<interrupt_lock> guard(lock);
	control_register.send(Control_Register::device_keyboard_deactive);
	control_register.send(Control_Register::device_mouse_deactive);

	//flush buffer
	while(isOutBufFull()) buffer.read();

	{//reconfigure
		control_register.send(Control_Register::command_byte_read);
		Control_Register::Configuration config;
		wait_outputBuf();
		config.data = buffer.read();
		config.firstPortInterrupt = false;
		config.secondPortInterrupt = false;
		config.firstPortTranslation = false;
		control_register.send(Control_Register::command_byte_write);
		wait_inputBuf();
		buffer.write(config.data);
	}
	//if (config.secondPortClock) debug <<"no mouse.\n";
	control_register.send(Control_Register::test_device_controller);
	wait_outputBuf();
	if (buffer.read() != 0x55)
	{
		//debug << "test and failed\n";
		return false;
	}
	/*check second port*/
	/**/
	control_register.send(Control_Register::command::device_keyboard_test);
	wait_outputBuf();
	const auto ss = buffer.read();
	if (ss != 00)
	{
		//debug << "test and failed. error values : " << (int)ss << "\n";
		return false;
	}

	control_register.send(Control_Register::command::device_keyboard_active);
	wait_inputBuf();
	buffer.write(Buffer::command::device_keyboard_active);
	wait_outputBuf();
	if (buffer.read() != Buffer::ack)
	{
		debug <<"failed activate\n";
		return false;
	}
	{
		control_register.send(Control_Register::command_byte_read);
		Control_Register::Configuration config;
		wait_outputBuf();
		config.data = buffer.read();
		config.firstPortInterrupt = true;
		//config.secondPortInterrupt = false;
		control_register.send(Control_Register::command_byte_write);
		wait_inputBuf();
		buffer.write(config.data);
	}
	wait_inputBuf();
	buffer.write(Buffer::command::reset_device);
	wait_outputBuf();
	auto d1 = buffer.read();
	uint8_t d2 = 0; 
	if(isOutBufFull()) d2 = buffer.read();
	if(d1 != 0xAA && d2 != 0xAA)
	{
		//debug << "keyboard init success\n";
		return false;
	}
	//debug << "failed";
	{
	wait_inputBuf();
	buffer.write(Buffer::set_or_get_scan_code_set);
	wait_outputBuf();
	auto b = buffer.read();
	if(b != Buffer::ack && b != Buffer::respend) return false;
	buffer.write(1); //set scancode set 1.
	b = buffer.read();
	if(b != Buffer::ack && b != Buffer::respend) return false;
	}
	return true;
}


bool ps2::changeKeyboardLED(ps2::KeyboardLED state)
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

key::ScanCode ps2::getScanCode()
{	
	return buffer.read();
}
#include<acpi/fadt.h>
#include<debug/logger.h>
bool ps2::Keyboard::initialize()
{
	if(initialized)
		return true;
	//https://uefi.org/sites/default/files/resources/ACPI_6_2.pdf page 191
	auto fadt = acpi::findFADT();
	if(fadt != nullptr && fadt->Revision > 1 && ! ((bool)(fadt->BootArchitectureFlags & (1<<1) )) )
	{
		logger << "ps2 not supported. flags : " << fadt->BootArchitectureFlags << "\n";
		return false;
	}
	//interrupt_lock t;
	//t.lock();
	bool success =	activate();
	if(success)
	{
		x86_64::register_handler(1,&irq_handler);
		initialized = true;
		//t.unlock();
		return true;
	}
	//t.unlock();
	return false;
}
size_t ps2::Keyboard::read(uint8_t * buf, size_t size)
{
	size_t offset = 0;
	while( (!keyQueue.empty()) && (offset < size)){
		buf[offset++] = keyQueue.pop();
	}
	return offset;
}
drv::IrqStatus ps2::KeyboardIRQHandler::topHalf()
{
	uint8_t sc;
	
	if (isOutBufFull()&&(!ps2::get_state_register().auxb))
	{
		context->keyboardQueueLock.lock();
		if (!context->keyQueue.full()) {
			sc = getScanCode();
			context->keyQueue.push(sc);
		}
		else
		{
			context->keyboardQueueLock.unlock();
			return drv::Irq_Status_Success;
		}
		context->keyboardQueueLock.unlock();
	}
	
	bool changed = true;
	switch (sc)
	{
	case 0x3A://Caps Lock press Scancode : 0x3A
		context->led.CapsLock ^= true;
		break;
	case 0x45://NumberLock press
		context->led.NumLock ^= true;
		break;
	case 0x46://ScrollLock press
		context->led.ScrollLock ^= true;
		break;
	default:
		changed = false;
	}
	if (changed)
		changeKeyboardLED(context->led);
	return drv::Irq_Status_Success;
}