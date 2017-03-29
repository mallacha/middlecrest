#include "../../../includes/service.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    locator::initialize();

    IRand * random = new simpleRandom();
    locator::provide(random);

    //Database service
    sqliteDatabase * database = new sqliteDatabase();
    locator::provide(database);

    //Character service
    characterPool * characters = new characterPool();
    locator::provideCharacterService(characters);
    characters->allocate(10, 1);

    //Display service
    IDisplay * display = new ascii();
    locator::provide(display);

    //Game service
    IGame * game = new middlecrest();
    locator::provide(game);
    game->initialize();

    //Unit testing service
    IUnitTest * unitTestHarness = new testHarness();
    locator::provide(unitTestHarness);

    unitTestHarness->run();

    system("PAUSE");
    return EXIT_SUCCESS;
}
