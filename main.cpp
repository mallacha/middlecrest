#include "core/includes/lib.h"
#include "game/includes/service.hpp"
#include "game/includes/configuration.hpp"
#include "game/includes/game.hpp"

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

    TRandomMersenne * random = new TRandomMersenne;
    locator::provide(random);
    /*IRand * rand1 = locator::getRNG();
    long temp = rand1->IRandom(1, 100);*/

    sqliteDatabase * database = new sqliteDatabase();
    locator::provide(database);

    asciiDisplay * display = new asciiDisplay();
    display->create();
    locator::provide(display);

    IGame * world = new game();
    locator::provide(world);

    characterPool * cPool = new characterPool();
    cPool->allocate(5, 3);
    cPool->createPC(); //TODO: characters tied to state object
    locator::provide(cPool); //Register
    IZoneCache * location = new zoneCache();
    locator::provide(location);
    /*MCLocation * location2 = locator::getLocation();
    location2->initialize();*/

    IEvents * eventsObject = new events();
    locator::provide(eventsObject);

    IDisplay * display2 = locator::getDisplay();

//TODO: characterPool is a little funky. Creates a characterPool type whereas character creates a IPool type. Both need to be IPool type
//      for service locator to give correct log message

while(1) {
    display2->draw();

    cPool->process();

    eventsObject->execute();
    world->incrementTurn();
    //cPool->createStatic();


    //system("PAUSE");
}
    //return EXIT_SUCCESS;
}
