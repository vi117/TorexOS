#pragma once
#include<algo/type_trait.h>
#include<algo/utility.h>
#include<algo/iterator.h>
#include<algo/new.h>
#include<alloc_policy/default_allocator.h>

namespace util
{
namespace list_detail
{
namespace detail
{
   /*SFINAE*/
 template <class NodeT> struct HasListTrait {
   template <class U>
   static true_type test1(U *I, decltype(I->getNext()) * = 0);
   template <class> static false_type &est1(...);
   template <class U>
   static true_type test2(U *I, decltype(I->getPrev()) * = 0);
   template <class> static false_type test2(...);
   template <class U>
   static true_type test3(U *I, decltype(I->setNext(&declval<NodeT&>())) * = 0);
   template <class> static false_type test3(...);
   template <class U>
   static true_type test4(U *I, decltype(I->setPrev(&declval<NodeT&>())) * = 0);
   template <class> static false_type test4(...);
   template <class U>
   static true_type test5(U *I, decltype(I->getDataPtr()) * = 0);
   template <class> static false_type test5(...);
   template <class U>
   static true_type test6(U *I, decltype(U::getNullNode()) * = 0);
   template <class> static false_type test6(...);

 public:
   static constexpr bool value = decltype(test1<NodeT>(nullptr))::value
   && decltype(test2<NodeT>(nullptr))::value
   && decltype(test3<NodeT>(nullptr))::value
   && decltype(test4<NodeT>(nullptr))::value
   && decltype(test5<NodeT>(nullptr))::value
   && decltype(test6<NodeT>(nullptr))::value;

 };
    
} // namespace detail

template<typename Ty>
struct list_node_trait
{
    using pointer = Ty *;
    inline pointer getNext() const noexcept {return list_next;}
    inline pointer getPrev() const noexcept {return list_prev;}
    inline void setNext(pointer ptr) noexcept {list_next = ptr;}
    inline void setPrev(pointer ptr) noexcept {list_prev = ptr;}
    static inline pointer getNullNode() noexcept {return nullptr;}

    pointer list_next;
    pointer list_prev;
};
template <typename Ty>
struct list_default_node : public list_node_trait<list_default_node<Ty>>
{
    using pointer = list_default_node *;
    using elem = Ty;

    elem * getDataPtr() {return addressof(data);}
    
    elem data;
};
template <>
struct list_default_node<void>  : public list_node_trait<list_default_node<void> >
{
    using pointer = list_default_node *;
    using elem = void;
    
    elem * getDataPtr() {return nullptr;}
};


#define list_assert(x) (void)(x)

template <typename Ty, typename node = list_default_node<Ty> >
class list_base
{
public:
    static_assert(detail::HasListTrait<node>::value,"no complete list trait");
    //static_assert(issame(),"elem");
    using elem = Ty;
    using node_type = node;
    using value_type = elem;
    class iterator
    {
      public:
        using value_type = elem;
        using pointer = elem *;
        using reference = elem &;
        using iterator_category = bidirectional_iterator_tag;

        constexpr explicit iterator() : pos(node::getNullNode()) {}
        constexpr iterator(node * t) : pos(t) {}
        constexpr iterator(const iterator &t) : pos(t.pos) {}

        iterator &operator++(){list_assert(pos != nullptr);pos = pos->getNext();return *this;}
        iterator &operator++(int){return operator++();}
        elem &operator*(){list_assert(pos != nullptr);return *pos->getDataPtr();}
        elem *operator->(){list_assert(pos != nullptr);return pos->getDataPtr();}
        iterator &operator--(){list_assert(pos != nullptr);pos = pos->getPrev();return *this;}
        bool operator==(const iterator &t) const{return (t.pos == pos);}
        bool operator!=(const iterator &t) const{return (t.pos != pos);}
        node * get() const {return pos;}
        private:
        node * pos;
    };
    
    constexpr node * null_node() const noexcept {return node::getNullNode();}
    
    constexpr explicit list_base() noexcept : head(null_node()), tail(null_node()) {}


    bool empty() const noexcept {return head == null_node();}
    //or tail==null_node().


    void pop_node(iterator obj) noexcept //List must have obj.
    {
        if (obj == front_iterator())
            pop_front_node();
        else if (obj == back_iterator())
            pop_back_node();
        else
            join_list(obj.get()->getPrev(), obj.get()->getNext());
    }

    iterator front_iterator() const noexcept {return iterator{head};}
    iterator back_iterator() const noexcept {return iterator{tail};}

    node * front_node() const noexcept{return head;}
    node * back_node() const noexcept{return tail;}

    iterator begin() const noexcept {return iterator{head};}
    iterator end() const noexcept {return iterator{null_node()};}

    elem & front() const noexcept { return *front_iterator();}
    elem & back() const noexcept { return *back_iterator();}

    bool _DebugChecked() const noexcept
    {//these comparison expression order is important.
        return head == null_node() || (
        tail != null_node() &&
        head->getPrev() == null_node() &&
        tail->getNext() == null_node());
    }

    void insert_front_node(iterator pos, node * obj) noexcept
    {
        if (pos == end()) push_back(obj);
        else if (pos == begin()) push_front(obj);
        else
        {
            iterator tmp = pos--;
            join_list(pos.get(), obj);
            join_list(obj, tmp.get());
        }
    }
    void push_front_node(node * obj) noexcept
    {
        if (empty())
        {
            tail = head = obj;
            obj->setNext(null_node());
            obj->setPrev(null_node());
        }
        else
        {
            join_list(obj, head);
            head = obj;
            head->setPrev(null_node());
        }
    }

    void push_back_node(node * obj) noexcept
    {
        if (empty())
        {
            tail = head = obj;
            obj->setNext(null_node());
            obj->setPrev(null_node());
        }
        else
        {
            join_list(tail, obj);
            tail = obj;
            tail->setNext(null_node());
        }
    }
    void pop_front_node() noexcept
    {
        if (!empty() && !orphan())
        {
            head = head->getNext();
            head->setPrev(null_node());
        }
        else
        {
            head = tail = null_node();
        }
    }
    void pop_back_node() noexcept
    {
        if (!empty() && !orphan())
        {
            tail = tail->getPrev();
            head->setNext(null_node());
        }
        else
        {
            head = tail = null_node();
        }
    }
    bool orphan() const noexcept
    {
        //if list is empty, return value is still true;
        return (tail == head);
        //if list not empty, equal
        //or return head->list_next == null_node()
        //or return tail->list_prev == null_node()
    }
    static void join_list(node * backpos, node *  afterpos) noexcept
    {
        backpos->setNext(afterpos);
        afterpos->setPrev(backpos);
    }
  private:
    node * head;
    node * tail;
};
template<typename Ty,typename Allocator>
class list : protected list_base<Ty>
{
public:
    using base = list_base<Ty>;
    using typename base::base;
    using typename base::empty;
    using typename base::begin;
    using typename base::end;
    using typename base::iterator;
    using typename base::front;
    using typename base::back;
    using typename base::node_type;
    using typename base::value_type;
    ~list()
    {
        while(base::empty()){
            auto it = base::front_node();
            base::pop_front_node();
            destroy_node(it);
        }
    }
    bool push_front(const value_type &element) noexcept
    {
        auto new_node = create_node(element);
        //allocation fail
        if (new_node == nullptr)
            return false;

        base::push_front_node(new_node);
        return true;
    }
    void pop_front()
    {
        auto n = base::pop_front_node();
        if (n != nullptr)
            destroy_node(n);
    }
    void erase(iterator it)
    {
        base::pop_node(it);
        destroy_node(it.get());
    }
private:
    template <typename... Args>
    node_type *create_node(Args &&... arg)
    {
        node_type * ret = alloc.allocate();
        ret->setNext(nullptr);
        ret->setPrev(nullptr);
        new (ret->getDataPtr()) value_type(forward<Args>(arg)...);
        return ret;
    }
    void destroy_node(node_type *ptr)
    {
        ptr->data.value_type::~value_type();
        alloc.deallocate(ptr);
    }
private:
    Allocator alloc;
};

} // namespace list_detail

template<typename Ty,typename Node>
using list_base = list_detail::list_base<Ty,Node>;
template<typename Ty,typename Allocator = default_allocator<util::list_detail::list_default_node< Ty > > >
using list = list_detail::list<Ty,Allocator>;

} // namespace util
