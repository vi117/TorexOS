#include<stddef.h>
#include<stdint.h>

#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

namespace text
{
    void * memset(void * pos, uint8_t value, size_t size);
    void * memcpy(void * dest,const void * src, size_t length);
    int memcmp(const void * ptr1,const void * ptr2,size_t length);
    size_t strlen(const char * str);
    void strcpy(char * dest, char * src);
} // text


#endif