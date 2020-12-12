#include "CustomTypeInterface.h"

OutOperator::OutOperator(DataStream &stream, char type,
                         const std::string &className) :
    m_OutStream(stream),
    m_IsSuccess(false)
{
    char tp = 0;
    if (!m_OutStream.readData(&tp) || tp != type) {
        m_IsSuccess = false;
        return ;
    }

    std::string cn;
    if (!m_OutStream.readVal(cn) || cn != className) {
        m_IsSuccess = false;
        return ;
    }

    m_IsSuccess = true;
}
