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
std::ostringstream ss;
};

BOOST_AUTO_TEST_CASE(PassTest)
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
