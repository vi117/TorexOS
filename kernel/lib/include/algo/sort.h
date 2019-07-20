#ifndef _ALGO_SORT_H_
#define _ALGO_SORT_H_
#include <algo/algobase.h>

namespace util
{/*
template <typename RandomAccessIter>
void quick_sort(RandomAccessIter begin, RandomAccessIter end)
{
    if(begin == end || begin + 1 == end) return;
    auto m = *begin;
    auto f = begin;
    auto l = end;
    l--;
    while (f < l){
        while (*f < m && f != l)
            f++;
        while (m < *l && f != l)
            l--;
        if (f == l)
            break;
        swap(*f, *l);
    }
    swap(*first,*f);
    quick_sort(first, f);
    quick_sort(f, last);
}*/

template<typename Iterator> //require *Iterator has ordered trait.
void selection_sort(Iterator begin, Iterator end){
    if(begin == end || begin + 1 == end) return;
    auto min_elem = min_element(begin,end);
    iter_swap(begin,min_elem);
    return selection_sort(++begin,end);
}

/* assume compare operation cost small and
 * move operation cost is relatively greater
 * than the compare operation.
*/
template <typename RandomAccessIter>
void insertion_sort(RandomAccessIter begin, RandomAccessIter end)
{
    if (begin == end || begin + 1 == end)
        return;
    auto sorted = begin;
    while (++sorted != end)
    {
        if (*sorted >= *(sorted - 1))
            continue;
        auto cur = sorted;
        auto temp = move(*sorted);
        do{
            cur--;
            if (*cur <= temp){
                cur++;
                break;
            }
            *(cur + 1) = move(*cur);
        } while (cur != begin);
        *cur = move(temp);
    }
}
template <typename RandomAccessIter,typename Compare>
void insertion_sort(RandomAccessIter begin, RandomAccessIter end, Compare pred)
{
    if (begin == end || begin + 1 == end)
        return;
    auto sorted = begin;
    while (++sorted != end)
    {
        if (pred(*(sorted - 1),*sorted))
            continue;
        auto cur = sorted;
        auto temp = move(*sorted);
        do{
            cur--;
            if (!pred(temp,*cur)){
                cur++;
                break;
            }
            *(cur + 1) = move(*cur);
        } while (cur != begin);
        *cur = move(temp);
    }
}


} // util
#endif