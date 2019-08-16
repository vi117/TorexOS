#pragma once
#include<stddef.h>

namespace util
{
    struct forward_iterator_tag {};
    struct bidirectional_iterator_tag : forward_iterator_tag {};
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct random_access_iterator_tag : bidirectional_iterator_tag{};

    template <typename Iterator> class iterator_traits{
        using difference_type = typename Iterator::difference_type;
        using value_type = typename Iterator::value_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
        using iterator_category = typename Iterator::iterator_category;
    };

    template <typename Iter> 
    class iterator_traits<Iter *>{
        using difference_type = ptrdiff_t;
        using value_type = Iter;
        using pointer = Iter *;
        using reference = Iter &;
        using iterator_category = random_access_iterator_tag;
    };

    template<typename Iter>
    class reverse_iterator{
        Iter it;
    public:
        using value_type = typename Iter::value_type;
        using pointer = typename Iter::pointer;
        using reference = typename Iter::reference;
        using iterator_category = typename Iter::iterator_category;

        reverse_iterator & operator++(){--it;return *this;}
        reverse_iterator & operator--(){++it;return *this;}
        reference & operator*(){return *it;}
    };
} // util
