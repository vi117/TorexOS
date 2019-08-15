#include<raw_ostream.h>
#include<compiler.h>

#define DEBUG

#ifndef _DEBUG_H_
#define _DEBUG_H_

class errstream : public text::raw_ostream{
public:
    constexpr explicit errstream():text::raw_ostream(){}
private:
    virtual void write_impl(const char *, size_t) override;
};

void panic(const char * message);
void assert_impl( const char * message,
                    const char * file,
                    const char * function,
                    unsigned long line);
void panicInException(int handler_num,const char * message);
#ifdef DEBUG
#define assert(x) ((void)((x)||(assert_impl(#x,__FILE__,__FUNCTION__,__LINE__))))
extern errstream debug;
#else
#define assert(x) (void(0))
class Dummy : public text::raw_ostream{
public:
template<typename Ty>
Dummy & operator << (Ty) override {return *this;}
private:
virtual void write_impl(const char * str ,size_t sz) override {}
};
Dummy debug;
#endif
#endif