#pragma once
#include<stddef.h>

namespace util
{
    struct bidirectional_iterator_tag {};
    struct forward_iterator_tag {};
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct random_access_iterator_tag {};

    template <typename Iterator> class iterator_traits{
        using difference_type = Iterator::difference_type;
        using value_type = Iterator::value_type;
        using pointer = Iterator::pointer;
        using reference = Iterator::reference;
        using iterator_category = Iterator::iterator_category;
    };

    template <typename Ty> 
    class iterator_traits<Ty *>{
        using difference_type = ptrdiff_t;
        using value_type = Ty;
        using pointer = Ty *;
        using reference = Ty &;
        using iterator_category = random_access_iterator_tag;
    };

} // util
