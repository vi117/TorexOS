#include <stddef.h>
#include <stdint.h>

namespace util
{
class bitmap{
public:
    bitmap(uint64_t * buf):data(buf){}
    struct proxy{
        uint64_t * data;
        size_t index;
        bool operator=(bool value){
            if (value){
                data[(index >> 6)] |= (1<< (index&0x3f));
            }
            else{
                data[(index >> 6)] &= ~(1<< (index&0x3f));
            }
            return value;
        }
    };
    bool operator[](size_t index) const{
        return data[(index >> 6)] & (1<< (index&0x3f));
    }
    proxy operator[](size_t index){
        return proxy{data,index};
    }
private:
    uint64_t * data;
};
} // util
