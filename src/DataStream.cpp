#include "DataStream.h"
#include "CustomTypeInterface.h"

DataStream::DataStream(std::unique_ptr<DataHeader> *header) :
    m_IsFirstWrite(true)
{
    if (header == nullptr) {
        m_Header.reset(new DataHeader);
    }
    else {
        m_Header.reset(header->release());
    }
}

DataStream::DataStream(const DataStream &stream)
{
    operator =(stream);
}

DataStream &DataStream::operator =(const DataStream &stream)
{
    if (&stream == this) {
        return *this;
    }

    m_Header.reset(new DataHeader);
    *m_Header = *stream.m_Header;

    m_DataBuffer = stream.m_DataBuffer;
    m_IsFirstWrite = stream.m_IsFirstWrite;

    return *this;
}

bool DataStream::operator ==(const DataStream &stream) const
{
    if (&stream == this) {
        return true;
    }

    if (m_Header.get() == stream.m_Header.get() &&
            m_DataBuffer == stream.m_DataBuffer) {
        return true;
    }

    return false;
}

void DataStream::clear()
{
    m_IsFirstWrite = true;
    m_DataBuffer.clear();
    m_Header->reset();
}

void DataStream::writeHeader()
{
    int headerLen = DataHeader::s_HeaderLen;

    writeData((char *)&(m_Header->m_TotalLen), sizeof(int));
    writeData((char *)&headerLen, sizeof(int));
    writeData((char *)&m_Header->m_Id, sizeof(int));

    m_Header->m_HeaderLen = headerLen;
}

void DataStream::writeData(const char *data, int len)
{
    if (len == 0) {
        return ;
    }

    if (m_IsFirstWrite) {
        m_IsFirstWrite = false;
        writeHeader();
    }

    m_DataBuffer.push(data, len);
    m_Header->m_TotalLen += len;
    memcpy(m_DataBuffer.begin(), &m_Header->m_TotalLen, sizeof(int));
}

DataStream &DataStream::operator<<(char val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(char val)
{
    char type = (char)DataType::Char;
    writeData((char *)&(type), sizeof(char));
    writeData(&val, sizeof(char));
}

DataStream &DataStream::operator<<(wchar_t val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(wchar_t val)
{
    char type = (char)DataType::WChar;
    writeData((char *)&(type), sizeof(char));
    writeData((char *)(&val), sizeof(wchar_t));
}

DataStream& DataStream::operator <<(bool val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(bool val)
{
    char type = (char)DataType::Boolean;
    writeData((char *)&(type), sizeof(char));
    writeData((char *)(&val), sizeof(bool));
}

DataStream& DataStream::operator <<(int val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(int val)
{
    char type = (char)DataType::Int;
    writeData((char *)&type, sizeof(char));
    writeData((char *)(&val), sizeof(int));
}

DataStream& DataStream::operator <<(unsigned int val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(unsigned int val)
{
    char type = (char)DataType::UInt;
    writeData((char *)&type, sizeof(char));
    writeData((char *)(&val), sizeof(unsigned int));
}

DataStream& DataStream::operator <<(int64_t val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(int64_t val)
{
    char type = (char)DataType::Int64;
    writeData((char *)&(type), sizeof(char));
    writeData((char *)(&val), sizeof(int64_t));
}

DataStream& DataStream::operator <<(double val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(double val)
{
    char type = (char)DataType::Double;
    writeData((char *)&(type), sizeof(char));
    writeData((char *)(&val), sizeof(double));
}

DataStream& DataStream::operator <<(const std::string &val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(const std::string &val)
{
    char type = (char)DataType::String;
    writeData((char *)&(type), sizeof(char));

    int size = val.size();
    writeVal(size);
    writeData(val.c_str(), size);
}

DataStream& DataStream::operator <<(const std::wstring &val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(const std::wstring &val)
{
    char type = (char)DataType::WString;
    writeData((char *)&(type), sizeof(char));

    int size = val.size();
    writeVal(size);
    writeData((const char *)val.c_str(), size * sizeof(wchar_t));
}

DataStream& DataStream::operator <<(CustomTypeInterface *val)
{
    WRITEBYSYMBOL(val);
}

void DataStream::writeVal(CustomTypeInterface *val)
{
    val->serialize(*this, (char)DataType::CustomType);
}

void DataStream::readHeader(const char *data)
{
    int *p = (int *)data;
    m_Header->m_TotalLen  = *p++;
    m_Header->m_HeaderLen = *p++;
    m_Header->m_Id        = *p++;

    m_Header->m_TotalLen -= m_Header->m_HeaderLen;
    m_Header->m_HeaderLen = 0;
}

bool DataStream::operator>>(char &val)
{
    return readVal(val);
}

bool DataStream::readVal(char &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::Char) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(wchar_t &val)
{
    return readVal(val);
}

bool DataStream::readVal(wchar_t &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::WChar) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(bool &val)
{
    return readVal(val);
}

bool DataStream::readVal(bool &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::Boolean) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(int &val)
{
    return readVal(val);
}

bool DataStream::readVal(int &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::Int) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(unsigned int &val)
{
    return readVal(val);
}

bool DataStream::readVal(unsigned int &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::UInt) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(int64_t &val)
{
    return readVal(val);
}

bool DataStream::readVal(int64_t &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::Int64) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(double &val)
{
    return readVal(val);
}

bool DataStream::readVal(double &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::Double) {
        return readData(&val);
    }

    return false;
}

bool DataStream::operator>>(std::string &val)
{
    return readVal(val);
}

bool DataStream::readVal(std::string &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::String) {
        int len = 0;
        if (readVal(len) && len > 0) {
            val.assign(m_DataBuffer.begin(), len);
            m_DataBuffer.removeFromFront(len);
            m_Header->m_TotalLen -= len;
        }

        return true;
    }

    return false;
}

bool DataStream::operator>>(std::wstring &val)
{
    return readVal(val);
}

bool DataStream::readVal(std::wstring &val)
{
    char type = 0;
    if (readData(&type) && type == (char)DataType::WString) {
        int len = 0;
        if (readVal(len) && len > 0) {
            val.assign((wchar_t*)m_DataBuffer.begin(), len);
            m_DataBuffer.removeFromFront(len * sizeof(wchar_t));
            m_Header->m_TotalLen -= len * sizeof(wchar_t);
        }

        return true;
    }

    return false;
}

bool DataStream::operator>>(CustomTypeInterface *val)
{
    return readVal(val);
}

bool DataStream::readVal(CustomTypeInterface *val)
{
    return val->deserialize(*this, (char)DataType::CustomType);
}

int DataStream::Serialize(char *buf) const
{
    int totalLen = m_Header->m_TotalLen;
    int size = m_DataBuffer.size();
    if (size <= 0 || totalLen == 0 || size != totalLen) {
        return 0;
    }

    memcpy(buf, m_DataBuffer.begin(), totalLen);
    return totalLen;
}

bool DataStream::Deserialize(const char *buf, int len)
{
    if (buf == nullptr || len <= 0) {
        return false;
    }

    readHeader(buf);

    m_DataBuffer.clear();
    m_DataBuffer.push(buf + DataHeader::s_HeaderLen, len - DataHeader::s_HeaderLen);
    return true;
}
