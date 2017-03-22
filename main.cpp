#include "includes/service.hpp"

#include "includes/screens.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    /*******************
      Pre-liftoff checks
    *******************/

    locator::initialize();

    #ifdef DEBUG
    //Check to see if debug mode is on. If so, log.
        ILogging * logger = locator::getLogging();
        logger->attn("Application running in development/debug mode.");
    #endif

    //Random number generator
    TRandomMersenne * random = new TRandomMersenne();
    locator::provide(random);

    //Database service
    sqliteDatabase * database = new sqliteDatabase();
    locator::provide(database);

    //Character service
    characterPool * characters = new characterPool();
    locator::provideCharacterService(characters);
    characters->allocate(10, 1);

    //Event service
    IEvents * eventsObject = new events();
    locator::provide(eventsObject);

    //Display service
    IDisplay * display = new ascii();
    locator::provide(display);

    //Game service
    IGame * game = new middlecrest();
    locator::provide(game);

    game = locator::getGame();
    game->initialize();


    //Begin game
    display->create();

    loadSavedGame();

    int ithElement = characters->createCharacter(PC);
    //id = characters->createCharacter(NPC);
    character * object = characters->getCharacterByPoolId(ithElement);

    display = locator::getDisplay();
    display->set(new cave());

    display->randomStartingLocation(ithElement);

    while(1) {
        display->draw();

        characters->process();

        eventsObject->execute();
    }

    game->save();

    //Create monsters and monster system
    //Finish world creation (need to connect maps/zones)*/

}
