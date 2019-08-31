#pragma once
#include<stdint.h>

namespace PIC8259
{
	void initialize(uint8_t IRQStartVector);
	void maskInterrupt(uint16_t IRQBitmask);
	void sendEOI(unsigned IRQNumber);
    uint16_t getMask();
	
    inline void disablePIC(){
        maskInterrupt(0xffff);
    }
}