#define BOOST_TEST_MODULE Stringutil_TEST
#include <boost/test/unit_test.hpp>
#include <stringutil.h>

BOOST_AUTO_TEST_CASE(PassTest)
{
    auto a = strlen("abcdefghijklmnopqrstuvwxyz");
    BOOST_CHECK_EQUAL(a, 26);
    auto b = strlen("123456789");
    BOOST_CHECK_EQUAL(b,9);
    
}
