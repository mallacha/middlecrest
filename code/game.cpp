#include "../includes/game.hpp"
#include "../includes/service.hpp"

using namespace std;

/**********************************************************
                           IGame
***********************************************************/

IGame::IGame()
{
    player_instance=0;
    running=1;
}

IGame::~IGame()
{
    player_instance=0;
    running=0;
}

void IGame::initialize()
{
     createWorldCellTable();
     createObjectTable();
}

void IGame::createWorldCellTable()
{
    saveDataTableCreation("SELECT name FROM sqlite_master WHERE type='table' AND name='maps' LIMIT 1;",
                          80,
                          "CREATE TABLE maps (mid INTEGER PRIMARY KEY AUTOINCREMENT, is_dungeon INTEGER, zone_x INTEGER, zone_y INTEGER, map_seed INTEGER, environ INTEGER DEFAULT 0);",
                          160);
}

void IGame::createObjectTable()
{
    saveDataTableCreation("SELECT name FROM sqlite_master WHERE type='table' AND name='objects' LIMIT 1;",
                          80,
                          "CREATE TABLE objects (oid INTEGER PRIMARY KEY AUTOINCREMENT, coord_x INTEGER DEFAULT -1, coord_y INTEGER DEFAULT -1, mid INTEGER, mid_linkage INTEGER, type TEXT DEFAULT 'unset', subtype TEXT DEFAULT 'none', FOREIGN KEY(mid_linkage) REFERENCES maps(mid));",
                          265);
}



/**********************************************************
                   Middlecrest game object
***********************************************************/

middlecrest::middlecrest()
{
}

middlecrest::~middlecrest()
{
}

void middlecrest::save()
{
    //Save characters
    characterPool * characters = locator::getCharacters();
    characters->save();
}

void middlecrest::initialize()
{
    IGame::initialize();

    /************************
     Create saves table
    ************************/

    saveDataTableCreation("SELECT name FROM sqlite_master WHERE type='table' AND name='saves' LIMIT 1;",
                          80,
                          "CREATE TABLE saves(id INTEGER PRIMARY KEY AUTOINCREMENT, cid INTEGER, name TEXT, FOREIGN KEY (cid) REFERENCES characters(id));",
                          130);

    /************************
     Save starting point
    ************************/

    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;

    //TODO: wcid=1 is used as a convention. Will need to match this to a saved game eventually
    //database->saveQuery(res, "SELECT wcid FROM world_cell WHERE wcid=1 LIMIT 1;", 50);
    database->saveQuery(res, "SELECT mid FROM maps WHERE mid=1 LIMIT 1;", 45);

    sqlite3_finalize(res);
}



/********************
 Null Object
********************/

nullGame::nullGame()
{
    logger=NULL;
}

nullGame::~nullGame()
{
    logger=NULL;
}

void nullGame::log()
{
    logger=locator::getLogging();
    logger->warn("Register service, using nullDatabase object.");
}
