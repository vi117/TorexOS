#define BOOST_TEST_MODULE my_forward_List_test
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <vector>
#include <random>
#include <datastruc/forward_list.h>

template<typename Ty>
struct commonAllocator
{
    Ty * allocate(){return (Ty *)malloc(sizeof(Ty));}
    void deallocate(Ty * ptr){free(ptr);}
};

BOOST_AUTO_TEST_CASE(PassTest1){
    util::forward_list<int,commonAllocator<util::forward_list_default_node<int> > > list;
    list.push_front(4);
    list.push_front(7);
    list.push_front(9);
    BOOST_ASSERT(list.front() == 9);
    list.pop_front();
    BOOST_ASSERT(list.front() == 7);
    list.pop_front();
    BOOST_ASSERT(list.front() == 4);
    list.pop_front();
}