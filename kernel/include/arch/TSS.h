#include<stdint.h>

#ifndef _TSS_H_
#define _TSS_H_
namespace x86_64
{
#pragma pack(push,1)
struct TSS
	{
		uint32_t reserved1;
		uint64_t RSP[3];
		uint64_t reserved2;
		/*linear address */
		uint64_t IST[7];
		uint64_t reserved3;
		uint16_t reserved;
		uint16_t IOMapBaseAddress;

		inline void IOMapDisable()
		{
			IOMapBaseAddress = 0xffff;
		}
	};
#pragma pack(pop)
} // namespace x86_64
#endif