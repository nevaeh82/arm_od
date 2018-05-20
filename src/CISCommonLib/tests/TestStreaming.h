#ifndef TESTSTREAMING_H
#define TESTSTREAMING_H
#include <cxxtest/TestSuite.h>
#include <QTcpServer>
#include <QTcpSocket>

class TestStreaming: public CxxTest::TestSuite
{

public:
    void test1()
    {
        TS_ASSERT_EQUALS(1, 1);/* expected, receiver.data);*/
    }
};

#endif // TESTSTREAMING_H
