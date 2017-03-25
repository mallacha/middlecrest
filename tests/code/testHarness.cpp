#include "../../includes/service.hpp"
#include "../../includes/utility.hpp"
#include <stdio.h>
#include <string.h>
#include <time.h>

testHarness::testHarness()
{
    testCount=0;
    succeses=0;
    failures=0;
    skips=0;
    codeCoverageTotal=0;
    codeCoverageCurrent=0;
    filesCovered=0;
}

testHarness::~testHarness()
{
    testCount=0;
    succeses=0;
    failures=0;
    skips=0;
    codeCoverageTotal=0;
    codeCoverageCurrent=0;
    filesCovered=0;
}

void testHarness::run()
{
    begin();

    RNGTests();
    basicPoolTests();

    printf("\nTesting Character Pool Functionality...\n");
    //With interface inherited interface this could look like:
    //ICharacterPool * charactersTest = new characterPoolTest();
    //charactersTest->allocate(1, 1);
    //Objects in game could inherit from ICharacterPool interface?
    ICharacterPoolTest * charactersTest = new ICharacterPoolTest();
    charactersTest->allocate(1, 1);

    //Files covered
    filesCovered += 0.5; //character pool partially written

    displayTests();
    mapTests();

    stop();

    //Files covered
    filesCovered += 2.5; //service.cpp, logging.cpp, database.cpp (partially written)

    report();
}

void testHarness::add(const char * title)
{
    printf("%s", title);

    int length = 60;
    for(int size = length - strlen(title); size >0; --size) {
        printf(".");
    }

    ++testCount;
}

void testHarness::displayResult(short status)
{
    switch(status) {
        case SKIPPED: printf("skipped\n"); ++skips; break;
        case SUCCESS: printf("success\n"); ++succeses; break;
        default: printf("failure\n"); ++failures;
    }
}

void testHarness::RNGTests()
{
    //Lines of code: RandInit: 9, IRandom 7, constructor 4
    incCoverageTotal(22);

    printf("Testing Random Number Generation...\n");
    IRand * random = new TRandomMersenneTest();
    displayResult(random->RandomInit(200));
    displayResult(random->IRandom(1, 50));

    //All lines covered sufficiently
    incCoverageCurrent(22);

    //Any cases missed?

    //Files covered
    filesCovered += 1.3; //RNG and utilities
}

void testHarness::basicPoolTests()
{
    //Lines of code: allocate: 13, setSize 33
    incCoverageTotal(46);

    printf("\nTesting Basic Pool Functionality...\n");
    IPool * testPool = new IPoolTest();
    testPool->allocate(0, 0);

    //All lines covered sufficiently
    incCoverageCurrent(46);

    //any cases missed?
    //TODO: what happens if you only allocate overflow? Kind of tested before but want better test, especially for character pools
    incCoverageTotal(1);

    //Files covered
    ++filesCovered; //basic pools
}

void testHarness::mapTests()
{
    printf("\nTesting Map Algorithms...\n");
    IMapTest * mapTest = new IMapTest();
    int mapGenerations = mapTest->generate();

    printf("Map generations: %d\nPercentge of failure: Less than %.6f%%\n", mapGenerations, 1/(float)mapGenerations);

    //Code coverage
    ++filesCovered; //drawing algorithm doesn't need to be tested automatically?
    filesCovered += 0.3; //partial utilities
    incCoverageTotal(319);
    incCoverageCurrent(302); //~cave not tested yet
    incCoverageTotal(8); //setTimer + checkTimerFinished
    incCoverageCurrent(8);
}

void testHarness::displayTests()
{
/*    printf("\nTesting Display Algorithms...\n");
    IDisplay * displayTest = new IDisplayTest();
    int generations = displayTest->randomStartingLocation();

    printf("Start location generations: %d\nPercentge of failure: Less than %.9f%%\n", generations, 1/(float)generations);

    //Code coverage
    ++filesCovered; //ascii.cpp
    incCoverageTotal(13);
    incCoverageCurrent(13);*/
}

void testHarness::begin()
{
    start = clock();
}

void testHarness::stop()
{
    end = clock();
}

void testHarness::report()
{
    //Adding test cases into coverage
    incCoverageTotal(testCount);
    incCoverageCurrent(testCount);

    //current/total. We add a 5% margin for human error. Probably too small, since this is a manual measurement
    float filePercentage = (filesCovered / 19.00)+0.1; //-5% when at 100%, +10% prior because of assertion testing
    printf("\nEstimated Code Coverage: %3.1f%%\n", ((((float)codeCoverageCurrent / (float)codeCoverageTotal)*100)*filePercentage));
    printf("Tests: Total %d | Successes: %d | Skipped %d | Failures %d\n", testCount, succeses, skips, failures);
}

void nullUnitTest::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullUnitTest object.");
}
