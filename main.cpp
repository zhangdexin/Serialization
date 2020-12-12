#include <iostream>

#include "DataStream.h"
#include "CustomTypeInterface.h"

class Test : public CustomTypeInterface
{
public:
    SerializeAndDeserialize(Test, m_A * m_B);

public:
    int  m_A;
    bool m_B;
};

int main(int argc, char *argv[])
{
    char c1 = 'c';

    Test t;
    t.m_A = 1;
    t.m_B = false;

    DataStream stream;
    stream.writeVal(c1);
    stream.writeVal(&t);

    int size = stream.totalSize();
    char *data = new char[size];
    stream.Serialize(data);

    DataStream stream2;
    stream2.Deserialize(data, size);
    char c2;
    Test t2;

    stream2.readVal(c2);
    stream2.readVal(&t2);

    std::cout << c2 << t2.m_A << t2.m_B;

    return 0;
}
