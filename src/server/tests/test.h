#ifndef TEST_H
#define TEST_H

#include <cxxtest/TestSuite.h>
#include <QDebug>

class test : public CxxTest::TestSuite
{
public:
    void test1()
    {
        TS_ASSERT_EQUALS(true, true);
    }
};

#endif // TEST_H
