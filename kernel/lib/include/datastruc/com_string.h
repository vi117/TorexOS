#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stringutil.h>
#include <datastruc/string_view.h>

namespace util
{
#pragma pack(push, 1)
class com_string
{
public:
    static constexpr unsigned little_size = sizeof(unsigned) + sizeof(char *) + 16 + 4;

    using iterator = char *;
    using const_iterator = const char *;

    constexpr explicit com_string() : m_size(0),m_str{0,}{}
    template <size_t N>
    explicit com_string(const char (&s)[N]) : com_string(s, N) {}
    explicit com_string(const char *str) : com_string(str,text::strlen(str)){}
    explicit com_string(const char *str, size_t length);
    
    com_string(com_string && str);
    ~com_string(){deallocate();}

    com_string & operator=(const char * str){
        assign(str,text::strlen(str));
        return *this;
    }
    com_string & operator=(const string_view & sv){
        assign(sv);
        return *this;
    }
    iterator begin() { return data();}
    iterator end() {return data() + size();}
    const_iterator cbegin() const {return data();}
    const_iterator cend() const { return data() + size();}
    void push_back(char ch)
    {
        append(&ch,1);
    }
    void append(const string_view & sv){append(sv.data(),sv.size());}
    void append(const char * str,size_t length);
    void assign(const string_view & sv){assign(sv.data(),sv.size());}
    void assign(const char *str, size_t length);

    char *data() noexcept
    {
        return is_local() ? m_str : m_data;
    }
    const char * data() const noexcept
    {
        return is_local() ? m_str : m_data;
    }
    char & operator[](unsigned index){
        return this->data()[index];
    }
    char operator[](unsigned index) const{
        return this->data()[index];
    }
    bool operator==(const util::string_view & str)
    {
        if(size() == str.size())
            return 0 == text::memcmp(data(), str.data(),size());
        return false;
    }
    void clear(){
        if(!is_local()) deallocate();
        m_size = 0;
    }
    unsigned size() const noexcept{return m_size;}
    operator string_view() { return string_view(data(), m_size); }
private:
    void grow(unsigned size);
    void allocate(unsigned length);
    void deallocate();

    inline bool is_local() const { return m_size < little_size; }

    //Round up to the next highest power of 2
    static unsigned suitable_size(unsigned sz);

private:
    unsigned m_size;
    union {
        struct //if string length is long.
        {
            unsigned m_reserved;
            char *m_data;
        };
        char m_str[little_size];
    };
};
#pragma pack(pop)
} // namespace util
