#include "../includes/location.hpp"
#include "../includes/service.hpp"

using namespace std;

zone::zone()
{
    dimLvl=1; //levels
    dimRow=38; //rows
    dimCol=83; //columns
    grid = new tile**[dimLvl];

    //Instantiate array
    for(short l=0; l < dimLvl; ++l) {
        grid[l] = new tile*[dimRow];

        for(short x=0; x < dimRow; ++x) {
            grid[l][x] = new tile[dimCol];
        }
        /******************
          No initial values
         ******************/
    }
}

zone::~zone()
{
    //Map deletion
    for(short l=0; l < dimLvl; ++l) {
        for(short x=0; x < dimRow; ++x) {
            delete[] grid[l][x];
        }

        delete[] grid[l];
    }

    delete[] grid;

    //Variables
    dimRow=0;
    dimCol=0;
    dimLvl=0;
    grid=NULL;
}

void zone::clearTiles()
{
    for(unsigned short r=1; r < getDimRow(); ++r) {
        for(unsigned short c=1; c < getDimCol(); ++c) {
            grid[0][r][c].clear();
        }
    }
}

void zoneCacheObject::setLastAccess()
{
    IGame * world = locator::getWorld();
    lastAccess=world->getTurn();
}

zoneCache::zoneCache()
{
    zones.reserve(ZONE_CACHE_SIZE);
    unsigned short i=0;
    do {
        zones[i] = new zoneCacheObject();
        ++i;
    }while(i < ZONE_CACHE_SIZE);

    //Validate existence of location table
    IGame * world = locator::getWorld();
    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;

    database->saveQuery(res, "SELECT name FROM sqlite_master WHERE type='table' AND name='location' LIMIT 1;", 78);

    //Create table, if needed
    if(sqlite3_column_type(res, 0) == 5) {
    //If result row is null (i.e. no table, column type is 5)
        sqlite3_finalize(res);
        database->saveQuery(res, "CREATE TABLE location (id INTEGER PRIMARY KEY AUTOINCREMENT, seed INTEGER, lastAccess INTEGER, mapLocationX INTEGER, mapLocationY INTEGER, areaLocationX INTEGER, areaLocationY INTEGER);", 185);
    }

    sqlite3_finalize(res);

    insertZone();

    locationObj = new cave();
    locationObj->generate();
}

zoneCache::~zoneCache()
{
    zones.clear();
}

zone * zoneCache::getZone()
{
    //grabs game service, grabs character service
    IGame * world = locator::getWorld();
    characterPool * characters = locator::getCharacters();
    character * pool = characters->getPool();
    character characterObj = pool[world->getPCPoolElement()];
    IDatabase * database = locator::getDatabase();
    unsigned int seed=0;
    unsigned short i=world->getZoneElement(); //array iterator

    if(zones[world->getZoneElement()]->getID() == characterObj.getZoneID()) {
    //Access zone in cache using index and id
    //Note: World object stores the element where the zone is stored in cache.
    //      Character object and cache both store the zone id. The zone id
    //      stored with character is compared against the cached value to
    //      verify the zone stored in cache is correct and current.
        //Set stuff
    } else {
    //Zone values not correct. Search for zone elsewhere in cache.
        unsigned short max=zones.size();

        i=0;
        while(zones[i]->getID() != characterObj.getZoneID() && i < max) {
        //Search array for zone ID. Terminates when found or array length exceeded
            ++i;
        }

        if(zones[i]->getID() == characterObj.getZoneID()) {
        //Found zone id
            world->setZoneElement(i); //Update location of zone in cache
            seed = zones[i]->getSeed();
        } else {
        //TODO: all this goes into functions
        //Not found:
          sqlite3_stmt* res;
          database->saveQuery(res, "SELECT seed FROM location WHERE id=%d LIMIT 1;", 55, characterObj.getZoneID());

          if(sqlite3_column_type(res, 0) != 5) {
          //Entry found. Result row is not null.
              seed = (int)sqlite3_column_int(res, 0);
          } else {
          //Entry not found in db. Create one.
            sqlite3_finalize(res);
            database->saveQuery(res, "INSERT INTO location SET(seed, lastAccess, mapLocationX, mapLocationY, areaLocationX, areaLocationY) VALUES (%d, %d, %d, %d, %d, %d);", 75,
                                seed, world->getTurn(), world->getMapLocationX(), world->getMapLocationY(), world->getAreaLocationX(), world->getAreaLocationY());
            sqlite3_finalize(res);

            //Select id, seed from database (can get turn from game object)
            database->saveQuery(res, "SELECT id FROM location WHERE mapLocationX=%d, mapLocationY=%d, areaLocationX=%d, areaLocationY=%d;", 105,
                                world->getMapLocationX(), world->getMapLocationY(), world->getAreaLocationX(), world->getAreaLocationY());
          }

          //Update game object, update cache
          //sqlite3_finalize(res);
        }
    }

    //Set last access time for zone
    zones[i]->setLastAccess();

    //return locationObj using seed;
    locationObj->setSeed(seed);
    return locationObj;
}

zoneCacheObject::zoneCacheObject()
{
    id=0;
    lastAccess=0;
    seed=0;
    mapLocationX=0;
    mapLocationY=0;
    areaLocationX=0;
    areaLocationY=0;
}

zoneCacheObject::~zoneCacheObject()
{
    id=0;
    lastAccess=0;
    seed=0;
    mapLocationX=0;
    mapLocationY=0;
    areaLocationX=0;
    areaLocationY=0;
}

void zoneCache::insertZone()
{
    IGame * world = locator::getWorld();
    IDatabase * database = locator::getDatabase();
    characterPool * characters = locator::getCharacters();
    character * pool = characters->getPool();
    sqlite3_stmt* res;

    //TODO: search by lastAccess, lowest lastAccess replace
    database->saveQuery(res, "SELECT mapX, mapY, areaX, areaY FROM globals WHERE pc=%d LIMIT 1;", 70, world->getPcID());
    zones[0]->setMapLocationX((unsigned short)sqlite3_column_int(res, 0));
    zones[0]->setMapLocationY((unsigned short)sqlite3_column_int(res, 1));
    zones[0]->setAreaLocationX((unsigned short)sqlite3_column_int(res, 2));
    zones[0]->setAreaLocationY((unsigned short)sqlite3_column_int(res, 3));
    world->setZoneElement(0);

    sqlite3_finalize(res);
    database->saveQuery(res, "SELECT id, seed FROM location WHERE mapLocationX=%d AND mapLocationY=%d AND areaLocationX=%d AND areaLocationY=%d LIMIT 1;", 150,
                       zones[0]->getMapLocationX(), zones[0]->getMapLocationY(), zones[0]->getAreaLocationX(), zones[0]->getAreaLocationY());
    zones[0]->setID((unsigned long)sqlite3_column_int(res, 0));
    characters->setZoneID(world->getPCPoolElement(), (unsigned long)sqlite3_column_int(res, 0));
    //Set seed
    zones[0]->setLastAccess();

    sqlite3_finalize(res);
}

void nullZoneCache::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullZoneCache object.");
}

int cave::generate()
//Cave creation function
{
    short r=0;
    short c=0;
    short i=0;
    short count=0;
    short repeat=10; //Number of iterations
    short wallFreq=40; //Frequency of wall
    IRand * random = locator::getRNG();

    random->RandomInit(seed);
    do {
        /*********************************************************************************
          Initialization: Steps through each tile and randomly assigns a wall tile. The
          location always starts completely empty.
        **********************************************************************************/
        clearTiles();

        for(r=0; r < getDimRow(); ++r) {
            for(c=0; c < getDimCol(); ++c) {
                if(random->IRandom(0, 100) < wallFreq) {
                //Setting walls in bare location based on wall frequency
                    grid[0][r][c].setWall();
                }
            }
        }

        /*********************************************************************************
          Cave creation: Steps through each tile and counts the number of populated
          squares surroundng it. If a certain number of sigils surround the square, that
          square is turned into a sigil. If not, then there is a random chance the
          algorithm will dig the square out to create an empty/bare tile.
        **********************************************************************************/
        for(i=0; i < repeat; ++i) {
            for(r=1; r < getDimRow(); ++r) {
                for(c=1; c < getDimCol(); ++c) {
                    count=0;
                    if(grid[0][r+1][c].isWall()) {
                      ++count;
                    }
                    if(grid[0][r-1][c].isWall()) {
                      ++count;
                    }
                    if(grid[0][r][c+1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r][c-1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r+1][c+1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r+1][c-1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r-1][c+1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r-1][c-1].isWall()) {
                      ++count;
                    }

                    if(count > 4) { //At least 5 walls surrounding tile, create wall
                        grid[0][r][c].setWall();
                    }else{
                        if(random->IRandom(0, 100) < 20) {
                        //20% chance to clear tile
                            grid[0][r][c].clear();
                        }
                    }
                }
            }
        }

        /*********************************************************************************
          Clean-up: Takes most wall tiles that are only surrounded by 0-2 wall squares
          and digs them out. Otherwise, the algorithm leaves a bunch of single wall tiles
          around. The occasional single wall tile is fine.
        **********************************************************************************/
        for(r=1; r < getDimRow()-2; ++r) {
            for(c=1; c < getDimCol()-2; ++c) {
                if(grid[0][r][c].isWall()) {
                    count=0;
                    if(grid[0][r+1][c].isWall()) {
                      ++count;
                    }
                    if(grid[0][r-1][c].isWall()) {
                      ++count;
                    }
                    if(grid[0][r][c+1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r][c-1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r+1][c+1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r+1][c-1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r-1][c+1].isWall()) {
                      ++count;
                    }
                    if(grid[0][r-1][c-1].isWall()) {
                      ++count;
                    }

                    if(count < 3) { //If less than 2 wall tiles surrounding it, clear it.
                        grid[0][r][c].clear();
                    }
                }
            }
        }

        for(r=0; r < getDimRow(); ++r) {
            for(c=0; c < getDimCol(); ++c) {
                if(c == 0 || r == 0 || c == getDimCol()-1 || r == getDimRow()-1) {
                //Setting walls in bare location based on wall frequency
                    grid[0][r][c].setWall();
                }
            }
        }

        //Check for unconnected areas with floodFill() and fill them.
        //Returns 0 on success, when the ratio of empty tiles is correct.
    }while(0 == floodFill(0.45));

    return 1;
}

int cave::floodFill(float expectedRatio)
//Flood fills rooms with walls to fill in unconnected areas
{
    int*** gridTemp;
    short r;
    short c;
    unsigned short total=dimRow * dimCol;
    IRand * random = locator::getRNG();

    //Instantiate temporary gridTemp
    //TODO: Allocate and deallocate grid in generate() function and pass into this function
    gridTemp = new int**[dimLvl];
    for(unsigned short l=0; l < dimLvl; ++l) {
        gridTemp[l] = new int*[dimRow];

        for(r=0; r < dimRow; ++r) {
            gridTemp[l][r] = new int[dimCol];

            for(c=0; c < dimCol; ++c) {
                gridTemp[l][r][c]=0;
            }
        }
    }

    unsigned short wallCount=0;
    unsigned short floorCount=0;
    unsigned short attempts=0;
    unsigned short maxAttempts=5;
    float ratio = (float)floorCount / (float)total;

    while(ratio < expectedRatio && attempts != maxAttempts) {
    //Main loop. Continues to flood fill until empty tile count is
    //greater than the exprected ratio and attempts are less than the
    //maximum. If the maximum attempts is exceeded, the grid may not be
    //the best fit or the randomly selected points may not be choosing
    //a satisfactorily connected tile. Another grid will be generated.

        //Initialize gridTemp. Set walls and empty tiles.
        wallCount=0;
        for(c=0; c < dimCol; ++c) {
            for(r=0; r < dimRow; ++r) {
                if(grid[0][r][c].isWall()) {
                    gridTemp[0][r][c]=1;
                    ++wallCount;
                }else{
                    gridTemp[0][r][c]=0;
                }
            }
        }

        //Fill empty space. Calculate ratio. Keep track of attempts.
        floorCount=0;
        fill(gridTemp, random->IRandom(0, getDimRow()), random->IRandom(0, getDimCol()), wallCount, total, floorCount);
        ratio = (float)floorCount / (float)total;
        if(floorCount > 0 && ratio < expectedRatio) {
            ++attempts;
        }
    }

    unsigned short returnVal = 1;
    if(attempts > maxAttempts-1) {
        returnVal=0;
    }

    //Patch
    if(returnVal) {
        for(c=0; c < getDimCol(); ++c) {
            for(r=0; r < getDimRow(); ++r) {
                if(gridTemp[0][r][c]==0) {
                    grid[0][r][c].setWall();
                    ++wallCount;
                }
            }
        }

/*        #ifdef ASSERTION_TEST
            char message[110];
            snprintf(message, 110, "ASSERTION TEST: wallCount (%d) + floorCount (%d) == total (%d)", wallCount, floorCount, total);
            ILogging * logger = locator::getLogging();
            logger->trace(message);
            assert(wallCount + floorCount == total);
        #endif
        #ifdef DEBUG
            if(wallCount + floorCount != total) {
                ILogging * logger = locator::getLogging();
                logger->warn("Unconnected areas in grid.");
            }
        #endif*/
    }

    //gridTemp deletion
    for(unsigned short l=0; l < dimLvl; ++l) {
        for(r=0; r < dimRow; ++r) {
            delete[] gridTemp[l][r];
        }

        delete[] gridTemp[l];
    }

    delete[] gridTemp;

    return returnVal;
}

int cave::fill(int *** & gridTemp, long r, long c, unsigned short wallCount, unsigned short total,
                   unsigned short & floorCount)
{
    if(gridTemp[0][r][c] == 0) {
        ++floorCount;
        gridTemp[0][r][c] = 1; //already touched

        if(r+1 < getDimRow()) {
            fill(gridTemp, r+1, c, wallCount, total, floorCount);
        }
        if(r-1 > 1) {
            fill(gridTemp, r-1, c, wallCount, total, floorCount);
        }
        if(c+1 < getDimCol()) {
            fill(gridTemp, r, c+1, wallCount, total, floorCount);
        }
        if(c-1 > 1) {
            fill(gridTemp, r, c-1, wallCount, total, floorCount);
        }
    }
}
