#include "CharVec.h"

CharVec::CharVec() :
    m_Elements(nullptr),
    m_FirstFree(nullptr),
    m_Cap(nullptr)
{}

CharVec::CharVec(const CharVec &vec)
{
    auto newData = allocAndCopy(vec.begin(), vec.end());

    m_Elements  = newData.first;
    m_FirstFree = newData.second;
    m_Cap       = newData.second;
}

CharVec &CharVec::operator =(const CharVec &vec)
{
    auto newData = allocAndCopy(vec.begin(), vec.end());
    free();

    m_Elements  = newData.first;
    m_FirstFree = newData.second;
    m_Cap       = newData.second;

    return *this;
}

CharVec::~CharVec()
{
    free();
}

bool CharVec::operator ==(const CharVec &vec) const
{
    if (m_Elements == vec.m_Elements &&
            m_FirstFree == vec.m_FirstFree &&
            m_Cap == vec.m_Cap) {
        return true;
    }

    return false;
}

size_t CharVec::size() const
{
    return m_FirstFree - m_Elements;
}

size_t CharVec::capacity() const
{
    return m_Cap - m_Elements;
}

char *CharVec::begin() const
{
    return m_Elements;
}

char *CharVec::end() const
{
    return m_FirstFree;
}

void CharVec::push(const char *data, int len)
{
    if (len <= 0) {
        return ;
    }

    for (int i = 0; i < len; ++i) {
        push(data[i]);
    }
}

void CharVec::push(const std::string &str)
{
    push(str.c_str(), str.size());
}

void CharVec::push(char c)
{
    checkAndAlloc();
    m_Allocator.construct(m_FirstFree++, c);
}

void CharVec::removeFromFront(int len)
{
    if (len > size()) {
        return ;
    }

    char *from = m_Elements;
    char *to = m_Elements + len;
    m_Elements += len;

    for (int i = 0; i < len; ++i) {
        m_Allocator.destroy(--to);
    }

    m_Allocator.deallocate(from, m_Elements - from);
}

void CharVec::clear()
{
    free();

    m_Elements = nullptr;
    m_FirstFree = nullptr;
    m_Cap = nullptr;
}

void CharVec::checkAndAlloc()
{
    if (size() == capacity()) {
        reallocate();
    }
}

void CharVec::reallocate()
{
    auto newCapacity = size() ? 2 * size() : 1;
    auto newData = m_Allocator.allocate(newCapacity);

    auto dest = newData;
    auto ele = m_Elements;

    for (size_t i = 0; i != size(); ++i) {
        m_Allocator.construct(dest++, std::move(*ele++));
    }
    free();

    m_Elements  = newData;
    m_FirstFree = dest;
    m_Cap       = m_Elements + newCapacity;
}

void CharVec::free()
{
    if (m_Elements) {
        for (auto p = m_FirstFree; p != m_Elements;) {
            m_Allocator.destroy(--p);
        }

        m_Allocator.deallocate(m_Elements, m_Cap - m_Elements);
    }
}

std::pair<char *, char *> CharVec::allocAndCopy(char *begin, char *end)
{
    auto startPos = m_Allocator.allocate(end - begin);
    return {startPos, std::uninitialized_copy(begin, end, startPos)};
}
