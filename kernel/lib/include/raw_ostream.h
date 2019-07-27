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

typedef raw_ostream& reference ;

public:
  constexpr explicit raw_ostream() : start(nullptr), cur(nullptr), end(nullptr) {}
  explicit raw_ostream(char *buf, size_t size) : start(buf),cur(start), end(buf + size)  {}
  raw_ostream(const reference ) = delete;
  void operator=(const reference ) = delete;

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
  reference operator<<(char ch)
  {
    if (isBufferFull())
    {
      return write(ch);
    }
    *cur++ = ch;
    return *this;
  }

  reference operator<<(signed char ch)
  {
    if (isBufferFull())
    {
      return write(ch);
    }
    *cur++ = ch;
    return *this;
  }
  reference operator<<(unsigned char ch)
  {
    if (isBufferFull())
    {
      return write(ch);
    }
    *cur++ = ch;
    return *this;
  }
  reference write(const char *str, size_t length);
  inline reference operator<<(const char *str)
  {
    return write(str, text::strlen(str));
  }
  reference operator<<(unsigned long N);
  reference operator<<(long N);
  reference operator<<(unsigned long long N);
  reference operator<<(long long N);
  reference operator<<(const void *P);


  reference operator<<(unsigned int N)
  {
    return this->operator<<(static_cast<unsigned long>(N));
  }
  reference operator<<(int N)
  {
    return this->operator<<(static_cast<long>(N));
  }
  //reference operator<<(double N);

  reference write_hex(unsigned long long N);

  //reference indent(unsigned NumSpaces);

  /// write_zeros - Insert 'NumZeros' nulls.
  //reference write_zeros(unsigned NumZeros);
protected:
  virtual void write_impl(const char *str, size_t size) = 0;

private:
  void flush_nonempty();
  void copy_to_buffer(const char *str, size_t size);
  reference write(unsigned char);
};
} // namespace text

#endif