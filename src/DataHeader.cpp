#include "DataHeader.h"

DataHeader::DataHeader(int id) :
    m_Id(id),
    m_HeaderLen(0),
    m_TotalLen(0)
{
}

bool DataHeader::operator==(const DataHeader &header)
{
    if (this == &header) {
        return true;
    }

    if (header.m_Id == m_Id && m_HeaderLen == header.m_HeaderLen &&
            m_TotalLen == header.m_TotalLen) {
        return true;
    }

    return false;
}

void DataHeader::reset()
{
    m_Id = 0;
    m_HeaderLen = 0;
    m_TotalLen = 0;
}
