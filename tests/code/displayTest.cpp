#include "../../includes/service.hpp"
#include "../includes/displayTest.hpp"
#include "../../includes/utility.hpp"

using namespace std;

int IDisplayTest::randomStartingLocation()
{
    IUnitTest * unitTestHarness = locator::getUnitTests();

    characterPool * characters = locator::getCharacters();
    int ithElement = characters->createCharacter(PC);
    character * object = characters->getCharacterByPoolId(ithElement);

    IDisplay * display = locator::getDisplay();
    display->set(CAVE);

    unitTestHarness->add("Testing Starting Location Randomization for 1 second");

    int generations=0;
    timerSet(1); //in seconds
    do {
        display->randomStartingLocation(ithElement);
        ++generations;
    }while(!timerFinished());
    unitTestHarness->displayResult(SUCCESS);

    delete characters;
    delete object;

    return generations;
}
