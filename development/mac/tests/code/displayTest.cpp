#include "../../../../includes/service.hpp"
#include "../includes/displayTest.hpp"
#include "../../../../includes/utility.hpp"

using namespace std;

int IDisplayTest::randomStartingLocation()
{
    IUnitTest * unitTestHarness = locator::getUnitTests();

    characterPool * characters = locator::getCharacters();
    int index = characters->createCharacter(PC);

    IDisplay * display = locator::getDisplay();
    unsigned map_id = display->createGameMap(CAVE);
    display->set(map_id);

    unitTestHarness->add("Testing Starting Location Randomization for 1 second");

    int generations=0;
    timerSet(1); //in seconds
    do {
        characters->setRandomStartingLocation(index);
        ++generations;
    }while(!timerFinished());
    unitTestHarness->displayResult(SUCCESS);

    return generations;
}
