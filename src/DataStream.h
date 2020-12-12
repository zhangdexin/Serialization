#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <memory>
#include <map>
#include <list>
#include <vector>
#include <set>

#include "DataHeader.h"
#include "CharVec.h"

#define WRITEBYSYMBOL(val) do {    \
        writeVal(val);             \
        return *this;              \
    } while (0)                    \

class CustomTypeInterface;
class DataStream
{
public:
    DataStream(std::unique_ptr<DataHeader> *header = nullptr);
    DataStream(const DataStream &stream);

    DataStream& operator =(const DataStream &stream);

    enum class DataType : char {
        UnKnown,
        Boolean,
        Char,
        WChar,
        Int,
        UInt,
        Int64,
        Double,
        String,
        WString,
        Vector,
        List,
        Map,
        Set,
        CustomType,
    };

    bool operator == (const DataStream &stream) const;

    // 指数组里存放的数据
    int totalSize() const { return m_Header->m_TotalLen; }
    int headerSize() const { return m_Header->m_HeaderLen; }
    int dataSize() const {return m_Header->m_TotalLen - m_Header->m_HeaderLen;}

    void clear();

    // write
    void writeHeader();
    void writeData(const char *data, int len);

    DataStream& operator<<(char val);
    void writeVal(char val);

    DataStream& operator<<(wchar_t val);
    void writeVal(wchar_t val);

    DataStream& operator <<(bool val);
    void writeVal(bool val);

    DataStream& operator <<(int val);
    void writeVal(int val);

    DataStream& operator <<(unsigned int val);
    void writeVal(unsigned int val);

    DataStream& operator <<(int64_t val);
    void writeVal(int64_t val);

    DataStream& operator <<(double val);
    void writeVal(double val);

    DataStream& operator <<(const std::string &val);
    void writeVal(const std::string &val);

    DataStream& operator <<(const std::wstring &val);
    void writeVal(const std::wstring &val);

    DataStream& operator <<(CustomTypeInterface *val);
    void writeVal(CustomTypeInterface *val);

    template<typename T>
    DataStream& operator <<(const std::vector<T>& val);
    template<typename T>
    void writeVal(const std::vector<T>& val);

    template<typename T>
    DataStream& operator <<(const std::list<T>& val);
    template<typename T>
    void writeVal(const std::list<T>& val);

    template<typename T1, typename T2>
    DataStream& operator <<(const std::map<T1, T2>& val);
    template<typename T1, typename T2>
    void writeVal(const std::map<T1, T2>& val);

    template<typename T>
    DataStream& operator <<(const std::set<T>& val);
    template<typename T>
    void writeVal(const std::set<T>& val);

    // read
    void readHeader(const char *data);

    template<typename T>
    bool readData(T *val);

    bool operator>>(char &val);
    bool readVal(char &val);

    bool operator>>(wchar_t& val);
    bool readVal(wchar_t &val);

    bool operator>>(bool &val);
    bool readVal(bool &val);

    bool operator>>(int &val);
    bool readVal(int &val);

    bool operator>>(unsigned int &val);
    bool readVal(unsigned int &val);

    bool operator>>(int64_t &val);
    bool readVal(int64_t &val);

    bool operator>>(double &val);
    bool readVal(double &val);

    bool operator>>(std::string &val);
    bool readVal(std::string &val);

    bool operator>>(std::wstring &val);
    bool readVal(std::wstring &val);

    bool operator>>(CustomTypeInterface *val);
    bool readVal(CustomTypeInterface *val);

    template<typename T>
    bool operator>>(std::vector<T> &val);
    template<typename T>
    bool readVal(std::vector<T> &val);

    template<typename T>
    bool operator>>(std::list<T> &val);
    template<typename T>
    bool readVal(std::list<T> &val);

    template<typename T1, typename T2>
    bool operator>>(std::map<T1, T2> &val);
    template<typename T1, typename T2>
    bool readVal(std::map<T1, T2> &val);

    template<typename T>
    bool operator>>(std::set<T> &val);
    template<typename T>
    bool readVal(std::set<T> &val);

    // Serialize and Deserialize
    int Serialize(char *buf) const;
    bool Deserialize(const char *buf, int len);

private:
    std::unique_ptr<DataHeader> m_Header;
    CharVec                     m_DataBuffer;
    int                         m_IsFirstWrite;
};

template<typename T>
DataStream& DataStream::operator <<(const std::vector<T>& val)
{
    WRITEBYSYMBOL(val);
}

template<typename T>
void DataStream::writeVal(const std::vector<T>& val)
{
    char type = (char)DataType::Vector;
    writeData((char *)&(type), sizeof(char));

    int size = val.size();
    writeVal(size);

    for (int i = 0; i < size; ++i) {
        writeVal(val.at(i));
    }
}

template<typename T>
DataStream& DataStream::operator <<(const std::list<T>& val)
{
    WRITEBYSYMBOL(val);
}

template<typename T>
void DataStream::writeVal(const std::list<T>& val)
{
    char type = (char)DataType::List;
    writeData((char *)&(type), sizeof(char));

    int size = val.size();
    writeVal(size);

    for (int i = 0; i < size; ++i) {
        writeVal(val.at(i));
    }
}

template<typename T1, typename T2>
DataStream& DataStream::operator <<(const std::map<T1, T2>& val)
{
    WRITEBYSYMBOL(val);
}

template<typename T1, typename T2>
void DataStream::writeVal(const std::map<T1, T2>& val)
{
    char type = (char)DataType::Map;
    writeData((char *)&(type), sizeof(char));

    int size = val.size();
    writeVal(size);

    for (auto item : val) {
        writeVal(item.first);
        writeVal(item.second);
    }
}

template<typename T>
DataStream& DataStream::operator <<(const std::set<T>& val)
{
    WRITEBYSYMBOL(val);
}

template<typename T>
void DataStream::writeVal(const std::set<T>& val)
{
    char type = (char)DataType::Set;
    writeData((char *)&(type), sizeof(char));

    int size = val.size();
    writeVal(size);

    for (auto item : val) {
        writeVal(item);
    }
}

template<typename T>
bool DataStream::operator>>(std::vector<T> &val)
{
    return readVal(val);
}

template<typename T>
bool DataStream::readVal(std::vector<T> &val)
{
    char type = 0;
    if (!readData(&type) || type != (char)DataType::Vector) {
        return false;
    }

    int size = 0;
    if (!readVal(size) || size == 0) {
        return false;
    }

    val.clear();
    for (int i = 0; i < size; ++i) {
        T t;
        if (readVal(t)) {
            val.push_back(t);
        }
    }
}

template<typename T>
bool DataStream::operator>>(std::list<T> &val)
{
    return readVal(val);
}

template<typename T>
bool DataStream::readVal(std::list<T> &val)
{
    char type = 0;
    if (!readData(&type) || type != (char)DataType::List) {
        return false;
    }

    int size = 0;
    if (!readVal(size) || size == 0) {
        return false;
    }

    for (int i = 0; i < size; ++i) {
        T t;
        if (readVal(t)) {
            val.push_back(t);
        }
    }
}

template<typename T1, typename T2>
bool DataStream::operator>>(std::map<T1, T2> &val)
{
    return readVal(val);
}

template<typename T1, typename T2>
bool DataStream::readVal(std::map<T1, T2> &val)
{
    char type = 0;
    if (!readData(&type) || type != (char)DataType::Map) {
        return false;
    }

    int size = 0;
    if (!readVal(size) || size == 0) {
        return false;
    }

    for (int i = 0; i < size; ++i) {
        T1 t1;
        T2 t2;
        if (readVal(t1) && readVal(t2)) {
            val[t1] = t2;
        }
    }
}

template<typename T>
bool DataStream::operator>>(std::set<T> &val)
{
    return readVal(val);
}

template<typename T>
bool DataStream::readVal(std::set<T> &val)
{
    char type = 0;
    if (!readData(&type) || type != (char)DataType::Set) {
        return false;
    }

    int size = 0;
    if (!readVal(size) || size == 0) {
        return false;
    }

    for (int i = 0; i < size; ++i) {
        T t;
        if (readVal(t)) {
            val.insert(t);
        }
    }
}

template<typename T>
bool DataStream::readData(T *val)
{
    int size = m_DataBuffer.size();
    int count = sizeof(T);
    if (size < count) {
        return false;
    }

    *val = *((T*)m_DataBuffer.begin());
    m_DataBuffer.removeFromFront(count);
    m_Header->m_TotalLen -= count;

    return true;
}

#endif // DATASTREAM_H
