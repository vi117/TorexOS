#pragma once

#include <stddef.h>

namespace util
{
    template<typename Ty>
    class PoolAllocator{
    public:
        constexpr explicit PoolAllocator():head(nullptr){}
        struct Node{
            union
            {
                Node * next;
                Ty dummy;
            };
        };
        Ty * allocate(){
            return pop();
        }
        void deallocate(Ty * ptr){
            push(ptr);
        }
        void push(Ty * ptr){
            auto new_head = reinterpret_cast<Node *>(ptr);
            new_head->next = head;
            head = new_head;
        }
        Ty * pop(){
            auto ret = head;
            if(ret == nullptr) return nullptr;
            head = head->next;
            return &ret->dummy;
        }
    private:
        Node * head;
    };
} // util
