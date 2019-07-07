#define BOOST_TEST_MODULE Rawostream_test
#include <boost/test/unit_test.hpp>
#include <raw_ostream.h>
#include <string>
#include <sstream>

class s_rawstream : public text::raw_ostream{
private:
    virtual void write_impl(const char * str, size_t length) override {
        ss.write(str,length);
    }
public:
explicit s_rawstream(char *buf, size_t size) : text::raw_ostream(buf,size)  {}
explicit s_rawstream() : text::raw_ostream()  {}

std::ostringstream ss;
};

BOOST_AUTO_TEST_CASE(PassTest1)
{
    s_rawstream ss;
    ss << "test string";
    BOOST_CHECK_EQUAL(ss.ss.str(), std::string("test string"));
    for(int i =0;i<1000;i++){
        ss.ss.str("");
        ss.ss.clear();
        ss << i;
        BOOST_CHECK_EQUAL(ss.ss.str(), std::to_string(i));
    }
}
BOOST_AUTO_TEST_CASE(PassTest2){
    char * buf = new char [8];
    s_rawstream ss(buf,8);
    ss << "test string";
    ss.flush();
    BOOST_CHECK_EQUAL(ss.ss.str(), std::string("test string"));
    ss.ss.str("");
    ss.ss.clear();
    int i = 553355331;
    ss << i;
    ss.flush();
    BOOST_CHECK_EQUAL(ss.ss.str(), std::to_string(i));
    delete buf;
}