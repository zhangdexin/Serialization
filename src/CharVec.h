#ifndef CHARVEC_H
#define CHARVEC_H

#include <memory>

class CharVec
{
public:
    CharVec();
    CharVec(const CharVec &vec);
    CharVec &operator =(const CharVec &vec);
    ~CharVec();

    bool operator ==(const CharVec &vec) const;

    size_t size() const;
    size_t capacity() const;

    char *begin() const;
    char *end() const;

    void push(const char *data, int len);
    void push(const std::string &str);
    void push(char c);

    void removeFromFront(int len);

    void clear();

private:
    void checkAndAlloc();
    void reallocate();
    void free();

    std::pair<char *, char *> allocAndCopy(char *begin, char *end);

private:
    char                *m_Elements;   // 首元素
    char                *m_FirstFree;  // 最后一个实际元素之后的位置
    char                *m_Cap;        // 分配内存末尾之后的位置

    std::allocator<char> m_Allocator;  // 内存分配器
};

#endif // CHARVEC_H
