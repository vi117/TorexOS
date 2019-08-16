#include <intrins.h>
#pragma once
namespace ps2
{
	struct State_Register
	{
		typedef State_Register self;
		typedef self & reference;
		typedef self * pointer;

		bool outbuf : 1;//output buffer state
		bool inbuf : 1;//input buffer state

		bool sysf : 1;//system flag
		bool cmdd : 1;//command And Data
		bool keyl : 1;//keyboard lock status
		bool auxb : 1;//Output Ausiliary Device
		bool tim : 1;//General Time-Out

		bool pare : 1;//parity Error

		enum
		{
			port = 0x64,
		};
		State_Register(){}

		operator uint8_t()
		{
			return *(uint8_t *)this;
		}
		reference updata()
		{
			(*(uint8_t *)this) = inbyte(port);
			return *this;
		}
	};
	static State_Register mystate_register;
	inline State_Register get_state_register(){
		
		return (mystate_register.updata());
	}
	struct Control_Register
	{
		typedef Control_Register self;
		typedef self & reference;
		enum
		{
			port = 0x64,
		};
		enum command
		{
			device_keyboard_active = 0xAE,
			device_keyboard_deactive = 0xAD,
			device_mouse_active = 0xA8,
			device_mouse_deactive = 0xA7,
			ready_send_to_mouse = 0xd4,
			command_byte_read=0x20,
			command_byte_write=0x60,
		};
		reference send(uint8_t b)
		{
			outbyte(port, b);
			return *this;
		}
		reference operator<<(uint8_t b)
		{
			return send(b);
		}
	};

	static Control_Register control_register;

	struct Buffer
	{
		typedef Buffer self;
		typedef self & reference;
		enum
		{
			port = 0x60,
			ack = 0xFA,//success Command;
		};
		enum command
		{
			changeLED=0xED,
			device_keyboard_active=0xF4,
			device_mouse_active=0xF4,
		};
		reference write(uint8_t b)
		{
			outbyte(port, b);
			return *this;
		}
		reference operator<<(uint8_t b)
		{
			return write(b);
		}
		uint8_t read()
		{
			return inbyte(port);
		}
		reference operator>>(uint8_t & b)
		{
			b = read();
			return *this;
		}
	};

	static Buffer buffer;
}