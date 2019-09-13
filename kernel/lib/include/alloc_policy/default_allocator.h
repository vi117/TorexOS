#pragma once

namespace util
{
    template<typename Ty>
    struct default_allocator
    {
        Ty *allocate(){ return new Ty;}
        void deallocate(Ty * ptr){delete ptr;}
    };
    template<typename Ty>
    struct default_deleter
    {
        void operator()(Ty * ptr)
        {
            delete ptr;
        }
    };
} // namespace util
