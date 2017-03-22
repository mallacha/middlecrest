#include "../../includes/service.hpp"
#include "../includes/locationTest.hpp"

using namespace std;

int IMapTest::generate()
{
    IUnitTest * unitTestHarness = locator::getUnitTests();

    unitTestHarness->add("Testing Cave Generation for 10 seconds");

    IMap * dungeon = new cave();
    dungeon->generate();

    int mapGenerations = dungeon->mapGenerations;
    unitTestHarness->displayResult(SUCCESS);

    delete dungeon;

    return mapGenerations;
}
