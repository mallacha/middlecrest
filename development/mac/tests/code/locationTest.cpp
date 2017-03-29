#include "../../../../includes/service.hpp"
#include "../../../../includes/utility.hpp"
#include "../includes/locationTest.hpp"

using namespace std;

long int IMapTest::generate()
{
    IUnitTest * unitTestHarness = locator::getUnitTests();

    unitTestHarness->add("Testing Cave Generation for 10 seconds");

    IMap * dungeon = new cave();
    int mapGenerations = 0;

    timerSet(10); //in seconds
    do {
        dungeon->generate();
        ++mapGenerations;
    }while(!timerFinished());

    unitTestHarness->displayResult(SUCCESS);

    delete dungeon;

    return mapGenerations;
}
