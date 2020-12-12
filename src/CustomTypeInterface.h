#ifndef CUSTOMTYPEINTERFACE_H
#define CUSTOMTYPEINTERFACE_H

#include "DataStream.h"

class InOperator
{
public:
    explicit InOperator(DataStream &stream,
                        char type,
                        const std::string &className) :
        m_InStream(stream) {
        m_InStream.writeData(&type, sizeof(char));
        m_InStream << className;
    }

    InOperator(const InOperator &) = delete;
    InOperator& operator=(const InOperator &) = delete;

    template<typename T>
    InOperator& operator *(const T &param) {
        m_InStream << param;
        return *this;
    }

private:
    DataStream &m_InStream;
};

class OutOperator
{
public:
    explicit OutOperator(DataStream &stream,
                         char type,
                         const std::string &className);

    OutOperator(const OutOperator &) = delete;
    OutOperator& operator=(const OutOperator &) = delete;

    template<typename T>
    OutOperator& operator* (T &param) {
        if (!m_IsSuccess) {
            return *this;
        }

        if (!(m_OutStream >> param)) {
            m_IsSuccess = false;
        }

        return *this;
    }

    bool isSuccess() const {
        return m_IsSuccess;
    }

private:
    DataStream  &m_OutStream;
    bool         m_IsSuccess;
};

#define SerializeAndDeserialize(className, params)                  \
    void serialize(DataStream &stream, char type) const override    \
    {                                                               \
        InOperator in(stream, type, #className);                    \
        in * params;                                                \
    }                                                               \
                                                                    \
    bool deserialize(DataStream &stream, char type) override  \
    {                                                               \
        OutOperator out(stream, type, #className);                  \
        out * params;                                               \
        return out.isSuccess();                                     \
    }                                                               \

class CustomTypeInterface
{
public:
    virtual ~CustomTypeInterface() = default;

    virtual void serialize(DataStream &stream, char type) const = 0;
    virtual bool deserialize(DataStream &stream, char type) = 0;
};

#endif // CUSTOMTYPEINTERFACE_H
