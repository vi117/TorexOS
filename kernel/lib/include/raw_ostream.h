#include <stddef.h>
#ifndef _FORMAT_H_
#define _FORMAT_H_

namespace text
{
class raw_ostream
{
  private:
    char *start;
    char *cur;
    char *end;

  protected:
    virtual void write_impl(const char *str, size_t size);

  public:
    raw_ostream() : start(nullptr), end(nullptr), cur(nullptr) {}
    raw_ostream(char *buf, size_t size) : start(buf), end(buf + size), cur(start) {}
    raw_ostream(const raw_ostream &) = delete;
    void operator=(const raw_ostream &) = delete;

    void flush()
    {
        if (start != cur)
            flush_nonempty();
    }
    void setBuffered(char * buf, size_t size){
        start = buf;
        end = buf + size;
        cur = start;
    }
    char * setUnbuffered(){
        auto ret = start;
        start = end = cur = nullptr;
        return ret;
    }
    bool isBuffered(){return start != nullptr;}
    
    void flush_nonempty();
    void write();
};
} // namespace text

#endif