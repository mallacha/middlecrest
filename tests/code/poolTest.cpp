#include "../../includes/service.hpp"
#include "../includes/poolTest.hpp"

using namespace std;

int IPoolTest::allocate(int value1, int value2)
{
    IUnitTest * unitTestHarness = locator::getUnitTests();
    unitTestHarness->add("Testing Primary Pool Integer Overflow");

    IPool * pool1 = new IPool();
    int primarySize = USHRT_MAX+2;
    pool1->allocate(primarySize, 0);
    unitTestHarness->displayResult((short)(pool1->getPoolSize() == USHRT_MAX));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Primary Pool Integer Underflow");
    primarySize = -3;
    pool1->allocate(primarySize, 0);
    unitTestHarness->displayResult((short)(pool1->getPoolSize() == 0));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Overflow Pool Integer Overflow");
    int secondSize = USHRT_MAX+1;
    pool1->allocate(1, secondSize);
    unitTestHarness->displayResult((short)(pool1->getOverflowSize() == USHRT_MAX));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Overflow Pool Integer Underflow");
    secondSize = -5;
    pool1->allocate(1, secondSize);
    unitTestHarness->displayResult((short)(pool1->getOverflowSize() == 0));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Max Pool Size");
    primarySize = USHRT_MAX;
    secondSize = USHRT_MAX;
    pool1->allocate(primarySize, secondSize);
    unitTestHarness->displayResult((short)(pool1->getSize() == USHRT_MAX*2));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Max Pool Size v2 (overflowing one int)");
    primarySize = USHRT_MAX;
    secondSize = USHRT_MAX+1;
    pool1->allocate(primarySize, secondSize);
    unitTestHarness->displayResult((short)(pool1->getSize() == USHRT_MAX*2));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Zero Pool Size Allocation");
    primarySize = 0;
    secondSize = 3;
    pool1->allocate(primarySize, secondSize);
    unitTestHarness->displayResult((short)(pool1->getSize() == primarySize + secondSize));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Double Init Protection");
    primarySize = 66;
    secondSize = 3;
    pool1->allocate(primarySize, secondSize);
    pool1->allocate(primarySize+5, secondSize-3);
    unitTestHarness->displayResult((short)(pool1->getSize() == primarySize + secondSize));

    delete pool1;

    pool1 = new IPool();
    unitTestHarness->add("Testing Double Init Protection v2 (+Zero Init)");
    primarySize = 0;
    secondSize = 3;
    pool1->allocate(primarySize, secondSize);
    int primarySizeReal = 1;
    int secondSizeReal = 3;
    pool1->allocate(primarySizeReal, secondSizeReal);
    primarySize = 50;
    secondSize = 3;
    pool1->allocate(primarySize, secondSize);
    unitTestHarness->displayResult((short)(pool1->getSize() == primarySizeReal + secondSizeReal));

    delete pool1;
}
