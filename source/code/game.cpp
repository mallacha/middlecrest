#include "../includes/game.hpp"
#include "../includes/service.hpp"

using namespace std;

game::game()
{
    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;

    //TODO: validate existence of table
    //Validate existence of location table
    //database->saveQuery(res, "SELECT name FROM sqlite_master WHERE type='table' AND name='globals' LIMIT 1;", 77);

/*    //Create table, if needed
    //TODO:
    if(sqlite3_column_type(res, 0) == 5) {
    //If result row is null (i.e. no table, column type is 5)
        sqlite3_finalize(res);
        database->saveQuery(res, "CREATE TABLE location (id INTEGER PRIMARY KEY AUTOINCREMENT, seed INTEGER, lastAccess INTEGER);", 95);
    }

    sqlite3_finalize(res);*/

    database->saveQuery(res, "SELECT pc,turn,mapX,mapY,areaX,areaY FROM globals LIMIT 1;", 59);
    pcID = (int)sqlite3_column_int(res, 0);
    turn = (long)sqlite3_column_int(res, 1);
    mapLocationX = (unsigned short)sqlite3_column_int(res, 2);
    mapLocationY = (unsigned short)sqlite3_column_int(res, 3);
    areaLocationX = (unsigned short)sqlite3_column_int(res, 4);
    areaLocationY = (unsigned short)sqlite3_column_int(res, 5);
    sqlite3_finalize(res);

    setProvinceName();
}

game::~game()
{
    turn=0;
    pcID=0;
    mapLocationX=0;
    mapLocationY=0;
    areaLocationX=0;
    areaLocationY=0;
    if(provinceName) {
        delete [] provinceName;
        provinceName = NULL;
    }
}

void game::incrementTurn() { 
    ++turn;

    if(turn %10 == 0) {
    //Transactions are slow so update every tenth time
        IDatabase * database = locator::getDatabase();
        sqlite3_stmt* res;

        database->saveQuery(res, "UPDATE globals SET turn = %d WHERE pc = %d;", 50, turn, pcID);
        sqlite3_finalize(res);
    }
}

void game::setProvinceName()
{
    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;

    if(provinceName) {
        delete [] provinceName;
    }

    database->dataQuery(res, "SELECT name FROM provinces p INNER JOIN map_cell mc WHERE row=%d and col=%d and mc.province=p.map_code;", 110, mapLocationY, mapLocationX);

    if(sqlite3_column_type(res, 0) == 5) {
        provinceName = new char[strlen("Terra Incognita")+1];
        strcpy(provinceName, "Terra Incognita");
    } else {
        provinceName = new char[strlen((char *)sqlite3_column_text(res, 0))+1];
        strcpy(provinceName, (char *)sqlite3_column_text(res, 0));
    }

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
