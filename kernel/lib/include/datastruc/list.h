#pragma once
#include<meta/type_trait.h>
#include<algo/utility.h>

namespace util
{
namespace list
{
template <typename Ty>
struct list_default_node
{
    using pointer = Default_node *;
    using elem = Ty;

    pointer next;
    pointer prev;
    elem data;

    elem *get()
    {
        return &data;
    }
};
#define list_assert(x) (void)
template <typename Ty, typename Allocator, typename Node = list_default_node<Ty>>
class list
{
  public:
    typedef Ty elem;

    class iterator
    {
      public:
        using value_type = elem;
        using pointer = elem *;
        using reference = elem &;
        using iterator_category = bidirectional_iterator_tag;

        constexpr explicit iterator() : pos(nullptr) {}
        constexpr explicit iterator(pointer t) : pos(t) {}
        constexpr iterator(const iterator &t) : pos(t.pos) {}

        iterator &operator++()
        {
            list_assert(pos->next != nullptr);
            pos = pos->next;
            return *this;
        }
        iterator &operator++(int)
        {
            return operator++();
        }
        elem &operator*()
        {
            list_assert(pos != nullptr);
            return *pos->get();
        }
        elem *operator->()
        {
            list_assert(pos != nullptr);
            return pos->get();
        }
        iterator &operator--()
        {
            list_assert(pos != nullptr);
            pos = pos->prev;
            return *this;
        }
        bool operator==(const iterator &t) const
        {
            return (t.pos == pos);
        }
        bool operator!=(const iterator &t) const
        {
            return (t.pos != pos);
        }
        node * get() const { return pos; }

      private:
        node * pos;
    };

    constexpr explicit list() : head(nullptr), tail(nullptr) {}

    bool empty() const
    {
        return (head == nullptr);
    }


    void pop(iterator obj) //List must have obj.
    {
        if (obj == front())
        {
            pop_front();
        }
        else if (obj == back())
        {
            pop_back();
        }
        else
        {
            join_list(obj.getraw()->prev, obj.getraw()->next);
        }
    }

    iterator front() const
    {
        return iterator{head};
    }
    iterator back() const
    {
        return iterator{tail};
    }

    iterator begin() const
    {
        return iterator{head};
    }
    iterator end() const
    {
        return iterator{nullptr};
    }
    reverse_iterator rbegin() const
    {
        return reverse_iterator{tail};
    }
    reverse_iterator rend() const
    {
        return reverse_iterator{nullptr};
    }

    bool _DebugChecked()
    {
        if (head != nullptr)
        {
            if (tail == nullptr)
            {
                return false;
            }
            if (head->prev != nullptr)
            {
                return false;
            }
            if (tail->next != nullptr)
            {
                return false;
            }
        }
        return true;
    }

  private:
    void insert_front_impl(iterator pos, node * obj)
    {
        if (pos == end())
        {
            push_back(obj);
        }
        else if (pos == begin())
        {
            push_front(obj);
        }
        else
        {
            iterator tmp = pos--;
            join_list(pos.get(), obj);
            join_list(obj, tmp.get());
        }
    }
    void push_front_impl(node * obj)
    {
        if (empty())
        {
            tail = head = obj;
            obj->next = obj->prev = nullptr;
        }
        else
        {
            join_list(obj, head);
            head = obj;
            head->prev = nullptr;
        }
    }

    void push_back_impl(node * obj)
    {
        if (empty())
        {
            tail = head = obj;
            obj->next = obj->prev = nullptr;
        }
        else
        {
            join_list(tail, obj);
            tail = obj;
            tail->next = nullptr;
        }
    }
    void pop_front_impl()
    {
        if (!empty())
        {
            if (!orphan())
            {
                head = head->next;
                head->prev = nullptr;
            }
            else
            {
                head = tail = nullptr;
            }
        }
    }
    void pop_back_impl()
    {
        if (!empty())
        {
            if (!orphan())
            {
                tail = tail->prev;
                tail->next = nullptr;
            }
            else
            {
                head = tail = nullptr;
            }
        }
    }
    bool orphan() const
    {
        return (tail == head);
    }
    static void join_list(pointer backpos, pointer afterpos)
    {
        backpos->next = afterpos;
        afterpos->prev = backpos;
    }
    template<typename ... Args>
    pointer create_node(Args && ... arg){
        auto ret = alloc.allocate();
        return new (ret) elem(forward(arg)...);
    }
    void destory_node(node * ptr){
        ptr->elem::~elem();
        alloc.deallocate(ptr);
    }
  private:
    pointer head;
    pointer tail;
    Allocator alloc;
};

} // namespace list
} // namespace util
