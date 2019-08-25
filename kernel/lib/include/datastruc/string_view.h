#pragma once
#include<stddef.h>
#include<stringutil.h>

namespace util
{
    class string_view{
    public:
    using value_type = char;
    using pointer = const char *;
    using const_pointer = pointer;
    using iterator = pointer;
    using const_iterator = iterator;
    using reference = const char &;
    using const_reference = reference;
    using size_type = size_t;

    constexpr string_view():m_str(nullptr),m_size(0){};
    constexpr string_view(const string_view& other) noexcept = default;
    constexpr string_view(char * s, size_t count) noexcept :m_str(s),m_size(count) {}
    string_view(const char* s):m_str(s),m_size(text::strlen(s)){}
    template<size_t N>
    string_view(const char(&buf)[N]):m_str(buf),m_size(N){}

    string_view& operator=(const string_view&) noexcept = default;

    constexpr const_iterator begin() const noexcept {return m_str;}
    constexpr const_iterator end() const noexcept {return m_str+m_size;}
    constexpr const_iterator cbegin() const noexcept {return m_str;}
    constexpr const_iterator cend() const noexcept {return m_str+m_size;}

    constexpr const_reference operator[](size_t index) const {return at(index);}
    constexpr const_reference at(size_t index) const {return m_str[index];}
    constexpr const_reference front() const{return *begin();}
    constexpr const_reference back() const{return *(end()-1);}
    constexpr const_pointer data() const noexcept{return m_str;}

    constexpr size_t size()const noexcept{return m_size;}
    constexpr size_t length()const noexcept{return m_size;}

    [[nodiscard]] constexpr bool empty() const noexcept{return m_size == 0;}

    [[nodiscard]] bool operator!=(const string_view & view)
    {
        if (view.size() != size()) return true;
        return text::memcmp(m_str,view.m_str,size());
    }
    [[nodiscard]] bool operator==(const string_view & view)
    {
        return !this->operator!=(view);
    }
    
    private:
        const char * m_str;
        size_t m_size;
    };
} // namespace util
