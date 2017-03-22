#include "../includes/ascii.hpp"
#include "../includes/service.hpp"

using namespace std;

IDisplay::IDisplay()
{
    map = NULL;
}

IDisplay::~IDisplay()
{
    if(map) {
        delete map;
    }
}

void IDisplay::set(IMap * object)
{
    if(map) {
        delete map;
    }

    map = object;
}

int IDisplay::randomStartingLocation(int ithCharacter)
{
    tile *** dungeon = map->getGrid();
    characterPool * characters = locator::getCharacters();
    character * characterObject = characters->getCharacterByPoolId(ithCharacter);
    IRand * random = locator::getRNG();
    unsigned int x = random->IRandom(1, getDimX()-1);
    unsigned int y = random->IRandom(1, getDimY()-1);

    while(dungeon[0][y][x].isAttribute(IMPASSIBLE_ATTRIBUTE)) {
        x = random->IRandom(1, getDimX()-1);
        y = random->IRandom(1, getDimY()-1);
    }

    characterObject->setXCoord(x);
    characterObject->setYCoord(y);
}

void nullDisplay::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullDisplay object.");
}








ascii::ascii()
{
    my_map=NULL;
    my_loc=NULL;
    dimCol=83; //columns
    dimRow=38; //rows
}

ascii::~ascii()
{
    delete my_map;
    delete my_loc;
    dimCol=0;
    dimRow=0;
}

void ascii::create()
//Create display
{
    initscr(); //Start curses mode

    //Main screen set up
    cbreak(); // Line buffering disabled, Pass on everty thing to me
    keypad(stdscr, TRUE); //Main window will take character input
    noecho();  //Don't echo characters to screen
    resize_term(50, 125);  //Set terminal to desired size, height x width
    curs_set(0); //Invisible cursor
    start_color();

    my_map=newwin(40, 85, 2, 0); //Playing map (sometimes doubles as a menu)
    my_loc=newwin(2, 85, 0, 0);  //Location banner/window
}

void ascii::draw()
{
    if(map) {
        //Initialization
        unsigned short rows=0;
        unsigned int cols=0;
        unsigned short lvls=0;
        tile *** dungeon = map->getGrid();
        characterPool * characters = locator::getCharacters();
        character * characterObject = characters->getPool();
        character obj;

        for(rows=0; rows < dimRow; ++rows) {
            for(cols=0; cols < dimCol; ++cols) {
                if(dungeon[lvls][rows][cols].isAttribute(IMPASSIBLE_ATTRIBUTE)) {
                //Impassible
                    mvwprintw(my_map, 1+rows, 1+cols, "#");
                } else {
                //Passible
                    mvwprintw(my_map, 1+rows, 1+cols, ".");
                }
            }
        }


        unsigned int arraySize = characters->getSize();
    //    total_objects = characters->getTotalObjects();
    //    if(total_objects > 0) {
            unsigned long i=0;
            long lastActive = characters->getLastActive();

            do {
                obj = characterObject[i];

                if(obj.lifetimeObject.isActive()) {
                    mvwprintw(my_map, 1+obj.getYCoord(), 1+obj.getXCoord(), "%c", obj.getSigil());
                }

                ++i;
            }while(i <= lastActive && i < arraySize);
    //    }

/*        IGame * world = locator::getWorld();
        if(strcmp(world->getProvinceName(), "Terra Incognita") == 0) {
            mvwprintw(my_loc, 0, 0, "%s", world->getProvinceName());
        } else {
            mvwprintw(my_loc, 0, 0, "Brytholm, Province of %s: %s", world->getProvinceName(), "foo");
        }*/

        box(my_map, 0, 0);
        wnoutrefresh(my_map);
        wnoutrefresh(my_loc);

        doupdate();
    }
}
