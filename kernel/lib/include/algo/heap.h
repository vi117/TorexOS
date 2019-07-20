#pragma once

namespace util
{
    template<typename RandomAccessIterator> /*require randomacess*/
    /*or enable if<is_randomaccessiterator<RandomAccessIterator>::value>::type*/
    void make_heap(RandomAccessIterator begin,RandomAccessIterator end){
        make_heap_array(begin, end - begin);
    }
} // util
