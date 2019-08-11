#pragma once

#include <stddef.h>

namespace util
{
template <typename Ty>
class PoolAllocator
{
public:
    constexpr explicit PoolAllocator() : head(nullptr) {}
    union Node {
        Node *next;
        Ty dummy;
    };
    inline Ty *allocate()
    {
        return pop();
    }
    inline void deallocate(Ty *ptr)
    {
        push(ptr);
    }
    void push(Ty *ptr)
    {
        auto new_head = reinterpret_cast<Node *>(ptr);
        new_head->next = head;
        head = new_head;
    }
    Ty *pop()
    {
        auto ret = head;
        if (ret == nullptr)
            return nullptr;
        head = head->next;
        return &ret->dummy;
    }
    void add_memblock(Node *start, size_t length)
    {
        const auto count = length / sizeof(Node);
        if(count > 0){
            for (size_t i = 0; i < count; i++)
            {
                start[i].next = & start[i+1];
            }
            start[count - 1].next = head;
            head = start;
        }
    }

private:
    Node *head;
};
} // namespace util
