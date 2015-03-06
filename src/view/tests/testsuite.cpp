/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#define _CXXTEST_ABORT_TEST_ON_FAIL
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "teststreaming.h"

static TestStreaming suite_TestStreaming;

static CxxTest::List Tests_TestStreaming = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestStreaming( "teststreaming.h", 7, "TestStreaming", suite_TestStreaming, Tests_TestStreaming );

static class TestDescription_TestStreaming_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_TestStreaming_test1() : CxxTest::RealTestDescription( Tests_TestStreaming, suiteDescription_TestStreaming, 11, "test1" ) {}
 void runTest() { suite_TestStreaming.test1(); }
} testDescription_TestStreaming_test1;

#include <cxxtest/Root.cpp>
