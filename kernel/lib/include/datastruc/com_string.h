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
    explicit com_string(const char *str, size_t length)
    {
        if(!is_local()) allocate(length);
        text::memcpy(data(), str, length);
        m_size = length;
        data()[m_size] = '\0';
    }
    com_string(com_string && str){
        if(is_local())
        {
            if (str.is_local())
            {
                text::memcpy(m_str, str.m_str, str.m_size);
                m_size = str.m_size;
                return;
            }
        }
        else
        {   
            deallocate();
        }
        m_data = str.m_data;
        m_reserved = str.m_reserved;
        m_size = str.m_size;
        str.m_size = 0;
    }
    ~com_string()
    {
        deallocate();
    }

    com_string & operator=(const char * str){
        assign(str,text::strlen(str));
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
    void append(const string_view & sv){
        append(sv.data(),sv.size());
    }
    void append(const char * str,size_t length){
        if (is_local())
        {
            if(m_size + length < little_size)
                text::memcpy(m_str+m_size,str,length);
            else{
                allocate(m_size + length);
                text::memcpy(m_data, m_str, m_size);
                text::memcpy(m_data+m_size,str,length);
            }
        }
        else{
            if(m_reserved <= m_size + length)
                grow(m_size + length);
            text::memcpy(m_data+m_size,str,length);
        }
        m_size += length;
        data()[m_size] = '\0';
    }
    void assign(const string_view & sv){
        assign(sv.data(),sv.size());
    }
    void assign(const char *str, size_t length)
    {
        if(length < little_size || ((!is_local())&&(length < m_reserved))){
            m_size = length;
            text::memcpy(data(), str, length);
        }
        else
        {
            deallocate();
            allocate(length);
            text::memcpy(m_data, str, length);
        }
        data()[m_size] = '\0';
    }
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
    unsigned size() const noexcept{
        return m_size;
    }
    operator string_view() { return string_view(data(), m_size); }
private:
    void grow(unsigned sz){
        sz = suitable_size(sz);
        char * newbuf = new char [sz];
        text::memcpy(newbuf,m_data,m_size);
        m_reserved = sz;
        delete[] m_data;
        m_data = newbuf;
    }
    void allocate(unsigned length)
    {
        m_size = length;
        m_reserved = suitable_size(length);
        m_data = new char[m_reserved];
    }
    void deallocate(){
        if(!is_local()){
            delete[] m_data;
        }
    }

    inline bool is_local() const { return m_size < little_size; }

    //Round up to the next highest power of 2
    static unsigned suitable_size(unsigned sz)
    {
        sz |= sz >> 1;
        sz |= sz >> 2;
        sz |= sz >> 4;
        sz |= sz >> 8;
        sz |= sz >> 16;
        sz++;   
        return sz;
    }

private:
    unsigned m_size;
    union {
        struct //if string length is big.
        {
            unsigned m_reserved;
            char *m_data;
        };
        char m_str[little_size];
    };
};
#pragma pack(pop)
} // namespace util
