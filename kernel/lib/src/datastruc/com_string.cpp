#include <datastruc/com_string.h>

util::com_string::com_string(const char *str, size_t length)
{
    if (!is_local())
        allocate(length);
    text::memcpy(data(), str, length);
    m_size = length;
    data()[m_size] = '\0';
}
util::com_string::com_string(com_string &&str)
{
    if (is_local())
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

void util::com_string::append(const char *str, size_t length)
{
    if (is_local())
    {
        if (m_size + length < little_size)
            text::memcpy(m_str + m_size, str, length);
        else
        {
            allocate(m_size + length);
            text::memcpy(m_data, m_str, m_size);
            text::memcpy(m_data + m_size, str, length);
        }
    }
    else
    {
        if (m_reserved <= m_size + length)
            grow(m_size + length);
        text::memcpy(m_data + m_size, str, length);
    }
    m_size += length;
    data()[m_size] = '\0';
}

void util::com_string::assign(const char *str, size_t length)
{
    if (length < little_size || ((!is_local()) && (length < m_reserved)))
    {
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

void util::com_string::grow(unsigned sz)
{
    sz = suitable_size(sz);
    char *newbuf = new char[sz];
    text::memcpy(newbuf, m_data, m_size);
    m_reserved = sz;
    delete[] m_data;
    m_data = newbuf;
}
void util::com_string::allocate(unsigned length)
{
    m_size = length;
    m_reserved = suitable_size(length);
    m_data = new char[m_reserved];
}
void util::com_string::deallocate()
{
    if (!is_local())
    {
        delete[] m_data;
    }
}

unsigned util::com_string::suitable_size(unsigned sz)
{
    sz |= sz >> 1;
    sz |= sz >> 2;
    sz |= sz >> 4;
    sz |= sz >> 8;
    sz |= sz >> 16;
    sz++;
    return sz;
}