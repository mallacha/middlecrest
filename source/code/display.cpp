#include "../includes/display.hpp"
#include "../includes/service.hpp"
#include "../includes/game.hpp"

using namespace std;

asciiDisplay::asciiDisplay()
{
    my_map=NULL;
    my_loc=NULL;
}

void asciiDisplay::create()
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

void asciiDisplay::draw()
{
    //Initialization
    unsigned short total_objects=0;
    unsigned short rows=0;
    unsigned short cols=0;
    unsigned short lvls=0;
    IZoneCache * location = locator::getLocation();
    zone * zoneArea = location->getZone();
    tile *** dungeon = zoneArea->getGrid();
    tile thisTile;
    characterPool * characters = locator::getCharacters();
    character * characterObject = characters->getPool();
    character obj;

    for(rows=0; rows < (unsigned short)zoneArea->getDimRow(); ++rows) {
        for(cols=0; cols < (unsigned short)zoneArea->getDimCol(); ++cols) {
            thisTile = dungeon[lvls][rows][cols];

            if(thisTile.isAttribute(IMPASSIBLE_ATTRIBUTE)) {
            //Impassible
                mvwprintw(my_map, 1+rows, 1+cols, "#");
            } else {
            //Passible
                mvwprintw(my_map, 1+rows, 1+cols, ".");
            }
        }
    }

    total_objects = characters->getTotalObjects();
    if(total_objects > 0) {
        unsigned long i=0;
        obj = characterObject[i];

        do {
            mvwprintw(my_map, 1+obj.getYCoord(), 1+obj.getXCoord(), "%c", obj.getSigil());
            ++i;
        }while(i < total_objects);
    }

    IGame * world = locator::getWorld();
    if(strcmp(world->getProvinceName(), "Terra Incognita") == 0) {
        mvwprintw(my_loc, 0, 0, "%s", world->getProvinceName());
    } else {
        mvwprintw(my_loc, 0, 0, "Brytholm, Province of %s: %s", world->getProvinceName(), "foo");
    }

    //TODO: Display total objects as part of debugging

    box(my_map, 0, 0);
    wnoutrefresh(my_map);
    wnoutrefresh(my_loc);

    doupdate();
}

asciiDisplay::~asciiDisplay()
{
    delete my_map;
    delete my_loc;
}

void nullDisplay::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullDisplay object.");
}
