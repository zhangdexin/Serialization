#ifndef DATAHEADER_H
#define DATAHEADER_H

struct DataHeader
{
    DataHeader(int id = 0);

    bool operator==(const DataHeader &header);
    void reset();

    const static int s_HeaderLen = 3 * sizeof(int);

    int m_Id;
    int m_HeaderLen;
    int m_TotalLen;
};

#endif // DATAHEADER_H
