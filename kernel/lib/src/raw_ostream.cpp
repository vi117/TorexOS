#include <raw_ostream.h>

namespace text
{
template<typename T, size_t N>
 static int format_to_buffer(T Value, char (&Buffer)[N]) {
   char *EndPtr = Buffer + N;
   char *CurPtr = EndPtr;
 
   do {
     *--CurPtr = '0' + char(Value % 10);
     Value /= 10;
   } while (Value);
   return EndPtr - CurPtr;
 }
 
inline void write_unsigned(raw_ostream &os,uint64_t n){
    char buf[24];
    auto len = format_to_buffer(n,buf);
    os.write(buf+24-len,len);
}

inline void write_integer(raw_ostream &os, uint64_t n)
{
    write_unsigned(os,n);
}
inline void write_integer(raw_ostream &os, int64_t n)
{
    if(n<0){
        os << '-';
        n = -n;
    }
    write_unsigned(os,n);
}
static const char * hexTable = "0123456789abcdef";
inline void write_hex(raw_ostream &os, unsigned long long number)
{
    char buf[20];
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 17; 2 <= i; i--)
    {
        buf[i] = hexTable[number & 0x0F];
        number >>= 4;
    }
    buf[18] = '\0';
    os.write(buf,18);
}
raw_ostream &raw_ostream::operator<<(unsigned long N)
{
    write_integer(*this, static_cast<uint64_t>(N));
    return *this;
}

raw_ostream &raw_ostream::operator<<(long N)
{
    write_integer(*this, static_cast<int64_t>(N));
    return *this;
}

raw_ostream &raw_ostream::operator<<(unsigned long long N)
{
    write_integer(*this, static_cast<uint64_t>(N));
    return *this;
}

raw_ostream &raw_ostream::operator<<(long long N)
{
    write_integer(*this, static_cast<int64_t>(N));
    return *this;
}

raw_ostream &raw_ostream::write_hex(unsigned long long N)
{
    text::write_hex(*this, N);
    return *this;
}
raw_ostream &raw_ostream::operator<<(const void *P)
{
    text::write_hex(*this, (uintptr_t)P);
    return *this;
}

void raw_ostream::flush_nonempty()
{
    //assert(OutBufCur > OutBufStart && "Invalid call to flush_nonempty.");
    size_t length = cur - start;
    cur = start;
    write_impl(start, length);
}

raw_ostream &raw_ostream::write(unsigned char C)
{
    if (start == nullptr)
    {
        write_impl(reinterpret_cast<char *>(&C), 1);
        return *this;
    }
    flush_nonempty();
    *cur++ = C;
    return *this;
}
raw_ostream &raw_ostream::write(const char *str, size_t size)
{
    if ((size_t)(end - cur) < size)
    {
        if (start == nullptr)
        {
            write_impl(str, size);
            return *this;
        }

        size_t NumBytes = end - cur;

        if (cur == start)
        {
            //assert(NumBytes != 0 && "undefined behavior");
            size_t BytesRemaining = (size % NumBytes);
            size_t BytesToWrite = size - BytesRemaining;
            write_impl(str, BytesToWrite);
            copy_to_buffer(str + BytesToWrite, BytesRemaining);
            return *this;
        }

        copy_to_buffer(str, NumBytes);
        flush_nonempty();
        return write(str + NumBytes, size - NumBytes);
    }

    copy_to_buffer(str, size);

    return *this;
}
/*
 template <char C>
 static raw_ostream &write_padding(raw_ostream &OS, unsigned NumChars) {
   static const char Chars[] = {C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C,
                                C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C,
                                C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C,
                                C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C,
                                C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C};
 
   // Usually the indentation is small, handle it with a fastpath.
   if (NumChars < array_lengthof(Chars))
     return OS.write(Chars, NumChars);
 
   while (NumChars) {
     unsigned NumToWrite = std::min(NumChars,
                                    (unsigned)array_lengthof(Chars)-1);
     OS.write(Chars, NumToWrite);
     NumChars -= NumToWrite;
   }
   return OS;
 }
 
 /// indent - Insert 'NumSpaces' spaces.
 raw_ostream &raw_ostream::indent(unsigned NumSpaces) {
   return write_padding<' '>(*this, NumSpaces);
 }
 
 /// write_zeros - Insert 'NumZeros' nulls.
 raw_ostream &raw_ostream::write_zeros(unsigned NumZeros) {
   return write_padding<'\0'>(*this, NumZeros);
 }*/
void raw_ostream::copy_to_buffer(const char *ptr, size_t length)
{
    //assert(length <= size_t(end - cur) && "Buffer overrun!");

    // Handle short strings specially, memcpy isn't very good at very short
    // strings.
    switch (length)
    {
    case 4:
        cur[3] = ptr[3];[[fallthrough]];
    case 3:
        cur[2] = ptr[2];[[fallthrough]];
    case 2:
        cur[1] = ptr[1];[[fallthrough]];
    case 1:
        cur[0] = ptr[0];[[fallthrough]];
    case 0:
        break;
    default:
        memcpy(cur, ptr, length);
        break;
    }

    cur += length;
}
} // namespace text