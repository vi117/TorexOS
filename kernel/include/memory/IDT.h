#include <stdint.h>

#ifndef _IDT_H_
#define _IDT_H_
namespace
{
 typedef void(*IDT_Handler)(void);
#pragma pack(push,1)
	struct IDTR
	{
		uint16_t Limit;
		uint64_t BaseAddress;
		uint16_t padding;
	};

	struct IDT
	{
		uint16_t LowerHandlerOffset;
		uint16_t SegmentSelector;
		/*Interrupt Stack Table
		 *TSS's IST index. 
		 * if IST is zero, No StackSwitching */
		uint8_t IST : 3;
		uint8_t NullSection1 : 5;
			/* IDT 게이트 종류.
				- 0110 : 16bit interrupt Gate (다른 핸들러 수행 중 인터럽트 X)
				- 0111 : 16bit trap Gate		(다른 핸들러 수행 중 인터럽트 O)
				- 1110 : 32bit interrupt Gate
				- 1111 : 32bit trap Gate
				- 0101 : 32bit task Gate*/
		uint8_t Type : 4;
			//NullSection. 0으로 채워넣으면 됨.
		uint8_t NullSection2 : 1;
			/* Descriptor Privilege Level 의 약자.
			   * 0(Highest)~3(Lowest)의 범위를 가짐.
			   * CPL나 RPL과 조합되어 접근 권한을 제한하는 데 사용. */
		uint8_t DPL : 2;
			//현재 유효한 디스크럽터인지 표시.
			//1 이면 유효. 0 이면 유효하지 않은 디스크립터.
		bool Present : 1;
			
		uint16_t MiddleHandlerOffset;
		uint32_t HighHandlerOffset;
		uint32_t reserved;

		void SetHandler(IDT_Handler function_pointer)
		{
            auto addr = reinterpret_cast<uintptr_t>(function_pointer);
			LowerHandlerOffset = static_cast<uint16_t>(addr & 0xFFFF);
			MiddleHandlerOffset = static_cast<uint16_t>((addr >> 16) & 0xFFFF);
			HighHandlerOffset = static_cast<uint32_t>(addr >> 32);
		}

	};
    #pragma pack(pop)
}

#endif