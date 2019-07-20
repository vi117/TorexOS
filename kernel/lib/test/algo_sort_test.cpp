#define BOOST_TEST_MODULE SortAlgorithm_test
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <vector>
#include <random>
#include <algo/sort.h>

BOOST_AUTO_TEST_CASE(PassTest1){
    std::vector<int> vec = {5,3,2,1,6};
    util::insertion_sort(vec.begin(),vec.end());
    std::vector<int> sorted ={1,2,3,5,6};
    
    BOOST_CHECK(vec == sorted);
}

BOOST_AUTO_TEST_CASE(PassTest2){
    std::vector<int> vec;
    vec.reserve(50);
    std::random_device rd; std::mt19937 mersenne(rd());

    std::uniform_int_distribution<> dist(1,50);

    for(int i = 0; i< 50; i++){
        vec.push_back(dist(mersenne));
    }
    auto cmp = vec;
    util::insertion_sort(vec.begin(),vec.end());
    std::sort(cmp.begin(),cmp.end());

    BOOST_CHECK(cmp == vec);
}