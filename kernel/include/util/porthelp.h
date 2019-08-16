#include <intrins.h>
#pragma once

class IOPort {
public:
    uint8_t read8(){
        return inbyte(port);
    }
    void write8(uint8_t v){
        return outbyte(port,v);
    }
    IOPort & operator<<(uint8_t b)
	{
		write8(b);
        return *this;
	}
    IOPort & operator>>(uint8_t & b){
        b = read8();
        return *this;
    }
    const uint16_t port;
};