#pragma once
#include <algo/new.h>
#include <algo/base.h>
#include <algo/utility.h>
#include <datastruc/optional.h>

namespace util
{
template <typename Ty>
struct forward_list_default_node
{
    using pointer = forward_list_default_node *;
    pointer next;
    Ty data;
};
template <typename Ty, typename node_t>
class forward_list_base
{
    node_t *head;

public:
    constexpr explicit forward_list_base() : head(nullptr) {}

    constexpr bool empty() const { return head == nullptr; }

    void push_front_node(node_t *node)
    {
        node->next = head;
        head = node;
    }
    node_t *pop_front_node()
    {
        auto ret = head;
        if (!empty())
            head = head->next;
        return ret;
    }
    node_t *front_node() const { return head; }

    class iterator
    {
    public:
        constexpr explicit iterator() : pos(nullptr) {}
        constexpr explicit iterator(node_t *node_ptr) : pos(node_ptr) {}
        constexpr iterator(const iterator &rhs) : pos(rhs.pos) {}
        constexpr iterator(iterator &&rhs) : pos(rhs.pos) {}

        iterator &operator++() { pos = pos->next; return *this; }
        iterator &operator++(int) { return operator++(); }
        Ty &operator*() { return pos->data; }
        Ty *operator->() { return pos; }
        constexpr bool operator==(iterator const &rhs) const noexcept { return pos == rhs.pos; }
        constexpr bool operator!=(iterator const &rhs) const noexcept { return pos != rhs.pos; }

    private:
        node_t *pos;
    };

    iterator begin() noexcept { return iterator{head}; }
    iterator end() noexcept { return iterator{nullptr}; }
};

template <typename Ty, typename allocator, typename node = forward_list_default_node<Ty>>
class forward_list : private forward_list_base<Ty, node>
{
public:
    using base_type = forward_list_base<Ty, node>;
    using value_type = Ty;

    using base_type::empty;
    using base_type::iterator;
    using base_type::begin;
    using base_type::end;

    constexpr explicit forward_list() : head(nullptr), alloc() {}

    bool push_front(const value_type &element) noexcept
    {
        auto new_node = create_node(element);
        //allocation fail
        if (new_node == nullptr)
            return false;

        base_type::push_front_node(new_node);
        return true;
    }
    void pop_front()
    {
        auto n = base_type::pop_front_node();
        if (n != nullptr)
            destroy_node(n);
    }
    value_type & front() const{
        return base_type::front_node()->data
    }
    optional<value_type> front_opt() const
    {
        auto n = base_type::front_node();
        if (n != nullptr)
            return n->data;
        return nullopt;
    }
    value_type * front_pointer()
    {
        auto n = base_type::front_node();
        if (n != nullptr)
            return addressof(n->data);
        return nullptr;
    }

private:
    template <typename... Args>
    node *create_node(Args &&... arg)
    {
        auto ret = alloc.allocate();
        ret->next = nullptr;
        new (addressof(ret->data)) value_type(forward<Args>(arg)...);
        return ret;
    }
    void destroy_node(node *ptr)
    {
        ptr->data.value_type::~value_type();
        alloc.deallocate(ptr);
    }

private:
    node *head;
    allocator alloc;
};
} // namespace util
