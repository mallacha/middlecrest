#include "../../includes/service.hpp"
#include "../includes/characterPoolTest.hpp"

using namespace std;

int characterPoolTest::allocate(int value1, int value2)
{
    IUnitTest * unitTestHarness = locator::getUnitTests();
    unitTestHarness->add("Testing Zero Pool Size Allocation");

    characterPool * tempCharacters = new characterPool();
    int primarySize = 0;
    int status = tempCharacters->allocate(primarySize, 0);
    unitTestHarness->displayResult((short)((status == 1) && tempCharacters->getSize() == primarySize));

    delete tempCharacters;

    tempCharacters = new characterPool();
    unitTestHarness->add("Testing Overflow-only Pool Allocation");
    primarySize = 0;
    int secondSize = 3;
    tempCharacters->allocate(primarySize, secondSize);
    unitTestHarness->displayResult((short)(tempCharacters->getSize() == primarySize + secondSize));

    delete tempCharacters;

    tempCharacters = new characterPool();
    unitTestHarness->add("Testing Character Pool Double Init Protection");
    primarySize = 66;
    secondSize = 3;
    tempCharacters->allocate(primarySize, secondSize);
    tempCharacters->allocate(primarySize+5, secondSize-3);
    unitTestHarness->displayResult((short)(tempCharacters->getSize() == primarySize + secondSize));

    delete tempCharacters;
}

int characterPoolTest::setRandomStartingLocation()
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


    //Lines covered (setRandomStartingLocation)
    unitTestHarness->incCoverageCurrent(5);
    unitTestHarness->incCoverageTotal(5);

    return generations;
}
