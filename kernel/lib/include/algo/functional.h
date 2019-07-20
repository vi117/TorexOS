#pragma once 
namespace util
{

#define def_operator(name,op) \
    template <typename T> struct name {\
    auto operator() (const T& x, const T& y) -> decltype(x op y) const {return x op y;}\
    }

def_operator(less,<);
def_operator(greater,>);
def_operator(equal_to,==);
def_operator(not_equal_to,!=);
def_operator(gerater_equal,<=);
def_operator(less_equal,<=);

} // util