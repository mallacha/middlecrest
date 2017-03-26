#include "../includes/ascii.hpp"
#include "../includes/service.hpp"

using namespace std;

IDisplay::IDisplay()
{
    map = NULL;
    random_xy[0]=0;
    random_xy[1]=0;
}

IDisplay::~IDisplay()
{
    if(map) {
        delete map;
    }
    random_xy[0]=0;
    random_xy[1]=0;
}

void IDisplay::set(LocationType type)
{
    if(map) {
        delete map;
    }

    switch(type) {
        case 1: map = new cave(); break;
        default: map = new IMap();
    }
}

int * IDisplay::getRandomStartingLocation()
{
    IRand * random = locator::getRNG();
    tile *** dungeon = map->getGrid();

    random_xy[0] = random->IRandom(1, getDimX()-1);
    random_xy[1] = random->IRandom(1, getDimY()-1);
    while(dungeon[0][random_xy[1]][random_xy[0]].isAttribute(IMPASSIBLE_ATTRIBUTE)) {
        random_xy[0] = random->IRandom(1, getDimX()-1);
        random_xy[1] = random->IRandom(1, getDimY()-1);
    }

    return random_xy;
}

void IDisplay::generate(const unsigned int index)
{
    unsigned int map_id = map->generate(index);

    //TODO: for testing purposes
    unsigned int link = map->generate();
    map->linkMaps(link, map_id);
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
    //TODO: which map?

    if(map) {
        //Initialization
        int rows=0;
        int cols=0;
        int lvls=0;
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




        /****************************************************
                    Map links (please profile)
        ****************************************************/

        IDatabase * database = locator::getDatabase();
        sqlite3_stmt* res;

        database->saveQuery(res, "SELECT coord_x, coord_y FROM objects WHERE mid=1 AND subtype='link';", 80);
        cols = sqlite3_column_int(res, 0);
        rows = sqlite3_column_int(res, 1);
        sqlite3_finalize(res);

        if(rows == -1 || cols == -1) {
            getRandomStartingLocation(); //TODO: reuse existing random_xy values to avoid call?
            cols = random_xy[0];
            rows = random_xy[1];

            database->saveQuery(res, "UPDATE objects SET coord_x=%d, coord_y=%d WHERE mid=%d;", 70, cols, rows, 1);
            sqlite3_finalize(res);
        }

        mvwprintw(my_map, rows, cols, "O");

        /****************************************************
                             Finished
        ****************************************************/







        unsigned int arraySize = characters->getSize();
    //    total_objects = characters->getTotalObjects();
    //    if(total_objects > 0) {
            unsigned long i=0;
            unsigned long lastActive = characters->getLastActive();

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
