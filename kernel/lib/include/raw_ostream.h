#include <stddef.h>
#include <stringutil.h>

#ifndef _FORMAT_H_
#define _FORMAT_H_

namespace text
{
  //llvm raw_ostream 조금 고쳐서 사용.
class raw_ostream
{
private:
  char *start;
  char *cur;
  char *end;


public:
  explicit raw_ostream() : start(nullptr), cur(nullptr), end(nullptr) {}
  explicit raw_ostream(char *buf, size_t size) : start(buf),cur(start), end(buf + size)  {}
  raw_ostream(const raw_ostream &) = delete;
  void operator=(const raw_ostream &) = delete;

  void flush()
  {
    if (start != cur)
      flush_nonempty();
  }
  void setBuffered(char *buf, size_t size)
  {
    start = buf;
    end = buf + size;
    cur = start;
  }
  char *setUnbuffered()
  {
    auto ret = start;
    start = end = cur = nullptr;
    return ret;
  }
  bool isBuffered() { return start != nullptr; }
  bool isBufferFull()
  {
    return cur >= end;
  }
  raw_ostream &operator<<(char ch)
  {
    if (isBufferFull())
    {
      return write(ch);
    }
    *cur++ = ch;
    return *this;
  }

  raw_ostream &operator<<(signed char ch)
  {
    if (isBufferFull())
    {
      return write(ch);
    }
    *cur++ = ch;
    return *this;
  }
  raw_ostream &operator<<(unsigned char ch)
  {
    if (isBufferFull())
    {
      return write(ch);
    }
    *cur++ = ch;
    return *this;
  }
  raw_ostream &write(const char *str, size_t length);
  raw_ostream &operator<<(const char *str)
  {
    return write(str, text::strlen(str));
  }
  raw_ostream &operator<<(unsigned long N);
  raw_ostream &operator<<(long N);
  raw_ostream &operator<<(unsigned long long N);
  raw_ostream &operator<<(long long N);
  raw_ostream &operator<<(const void *P);


  raw_ostream &operator<<(unsigned int N)
  {
    return this->operator<<(static_cast<unsigned long>(N));
  }
  raw_ostream &operator<<(int N)
  {
    return this->operator<<(static_cast<long>(N));
  }
  //raw_ostream &operator<<(double N);

  raw_ostream &write_hex(unsigned long long N);

  //raw_ostream &indent(unsigned NumSpaces);

  /// write_zeros - Insert 'NumZeros' nulls.
  //raw_ostream &write_zeros(unsigned NumZeros);
protected:
  virtual void write_impl(const char *str, size_t size) = 0;

private:
  void flush_nonempty();
  void copy_to_buffer(const char *str, size_t size);
  raw_ostream &write(unsigned char);
};
} // namespace text

#endif