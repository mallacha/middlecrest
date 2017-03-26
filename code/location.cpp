#include "../includes/service.hpp"

using namespace std;

IMap::IMap()
{
    dimLvl=1;
    map_id=0;
    grid = new tile**[dimLvl];
    IDisplay * display = locator::getDisplay();
    const unsigned short dimRow = display->getDimY()+1;
    const unsigned short dimCol = display->getDimX()+1;

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

IMap::~IMap()
{
    map_id=0;
    dimLvl=0;
    //grid should be taken care of in dervied classes
}

cave::cave()
{
}

cave::~cave()
{
    IDisplay * display = locator::getDisplay();
    const unsigned short dimRow = display->getDimY()+1;

    //Map deletion
    for(short l=0; l < dimLvl; ++l) {
        for(short x=0; x < dimRow; ++x) {
            delete[] grid[l][x];
        }

        delete[] grid[l];
    }

    delete[] grid;

    //Variables
    map_id=0;
    dimLvl=0;
    grid=NULL;
}

void cave::linkMaps(unsigned int source, unsigned int destination)
{
    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;

    database->saveQuery(res, "INSERT INTO objects (mid, mid_linkage, type, subtype) VALUES (%d, %d, 'object', 'link');", 100, source, destination);
    sqlite3_finalize(res);
    database->saveQuery(res, "INSERT INTO objects (mid, mid_linkage, type, subtype) VALUES (%d, %d, 'object', 'link');", 100, destination, source);
    sqlite3_finalize(res);

    //success or failure will be logged for queries
}

unsigned int cave::generate(const unsigned int index)
//Generate a map based on character id and its location
{
    generate();

    //TODO: ultimately I want to get by object ids?
    characterPool * characters = locator::getCharacters();
    characters->setMapIdByPoolId(index, map_id);
    characters->save(index);

    return map_id;
}

unsigned int cave::generate()
//Cave creation function
{
    unsigned short r=0;
    unsigned short c=0;
    short i=0;
    short count=0;
    const short repeat=10; //Number of iterations: 10 = sea cave (something hollowed out by water, 2 = lava cave, 3 = ice/limestone cave, 1 = mud cave
    const short wallFreq=40; //Frequency of wall
    IRand * random = locator::getRNG();
    IDisplay * display = locator::getDisplay();
    random->RandomInit(rand());
    const unsigned short dimRow = display->getDimY();
    const unsigned short dimCol = display->getDimX();
    const unsigned short boundariesRow = dimRow+1;
    const unsigned short boundariesCol = dimCol+1;

                #ifndef UNIT_TEST
                //Temporary. Just here for development until
                //a full map generation algorithm is developed
                    IDatabase * database = locator::getDatabase();

                    sqlite3_stmt* res;
                    database->saveQuery(res, "INSERT INTO maps (is_dungeon, zone_x, zone_y, map_seed, environ) VALUES (1, 3, 11, %d, %d);", 150, random->getSeed(), CAVE);
                    sqlite3_finalize(res);

                    database->saveQuery(res, "SELECT mid FROM maps ORDER BY mid DESC LIMIT 1;", 50);
                    map_id = (unsigned int)sqlite3_column_int(res, 0);
                    sqlite3_finalize(res);
                #endif


    do {
        /*********************************************************************************
          Initialization: Steps through each tile, clears it, and randomly assigns a wall
          tile.
        **********************************************************************************/
        //clearTiles();

        r=boundariesRow;
        //for(r=0; r < boundariesRow; ++r) {
        do {
            --r;
            c=boundariesCol;
            //for(c=0; c < boundariesCol; ++c) {
            do {
                --c;
                grid[0][r][c].clear();
                if(random->IRandom(0, 100) < wallFreq) {
                //Setting walls in bare location based on wall frequency
                    grid[0][r][c].setWall();
                }
            }while(c);
        }while(r);

        /*********************************************************************************
          Cave creation: Steps through each tile and counts the number of populated
          squares surroundng it. If a certain number of sigils surround the square, that
          square is turned into a sigil. If not, then there is a random chance the
          algorithm will dig the square out to create an empty/bare tile.
        **********************************************************************************/
        i=repeat;
        //for(i=0; i < repeat; ++i) {
        do {
            --i;
            r=dimRow-1;
            do {
            //for(r=1; r < dimRow; ++r) {
                c=dimCol-1;
                //for(c=1; c < dimCol; ++c) {
                do {
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
                }while(--c);
            }while(--r);
        }while(i);

        /*********************************************************************************
          Clean-up: Takes most wall tiles that are only surrounded by 0-2 wall squares
          and digs them out. Otherwise, the algorithm leaves a bunch of single wall tiles
          around. The occasional single wall tile is fine.
        **********************************************************************************/
        r=dimRow-1;
        do {
        //for(r=1; r < dimRow; ++r) {
            c=dimCol-1;
            do {
            //for(c=1; c < dimCol; ++c) {
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
            }while(--c);
        }while(--r);



        for(r=0; r < boundariesRow; ++r) {
            for(c=0; c < boundariesCol; ++c) {
                if(c==0 || r==0 || c == dimCol || r == dimRow) {
                //Setting walls in bare location based on wall frequency
                    grid[0][r][c].setWall();
                }
            }
        }

    }while(0 == floodFill(0.45));

    return map_id; //doubles as a success (true > 0)
}

int cave::floodFill(float expectedRatio)
//Flood fills rooms with walls to fill in unconnected areas
{
    IRand * random = locator::getRNG();
    IDisplay * display = locator::getDisplay();
    const unsigned short dimRow = display->getDimY();
    const unsigned short dimCol = display->getDimX();
    const unsigned short boundariesRow = dimRow+1;
    const unsigned short boundariesCol = dimCol+1;
    int*** gridTemp;
    unsigned short r;
    unsigned short c;
    unsigned short total=boundariesRow * boundariesCol;

    //Instantiate temporary gridTemp
    //TODO: Allocate and deallocate grid in generate() function and pass into this function
    gridTemp = new int**[dimLvl];
    for(unsigned short l=0; l < dimLvl; ++l) {
        gridTemp[l] = new int*[boundariesRow];

        for(r=0; r < boundariesRow; ++r) {
            gridTemp[l][r] = new int[boundariesCol];

            for(c=0; c < boundariesCol; ++c) {
                gridTemp[l][r][c]=0;
            }
        }
    }

    unsigned short wallCount=0;
    unsigned short floorCount=0;
    unsigned short attempts=0;
    const unsigned short maxAttempts=5;
    float ratio = (float)floorCount / (float)total;

    while(ratio < expectedRatio && attempts != maxAttempts) {
    //Main loop. Continues to flood fill until empty tile count is
    //greater than the exprected ratio and attempts are less than the
    //maximum. If the maximum attempts is exceeded, the grid may not be
    //the best fit or the randomly selected points may not be choosing
    //a satisfactorily connected tile. Another grid will be generated.

        //Initialize gridTemp. Set walls and empty tiles.
        wallCount=0;
        r=boundariesRow;
        do {
        //for(r=0; r < boundariesRow; ++r) {
            --r;
            c=boundariesCol;
            do {
            //for(c=0; c < boundariesCol; ++c) {
                --c;
                if(grid[0][r][c].isWall()) {
                    gridTemp[0][r][c]=1;
                    ++wallCount;
                }else{
                    gridTemp[0][r][c]=0;
                }
            }while(c);
        }while(r);

        //Fill empty space. Calculate ratio. Keep track of attempts.
        floorCount=0;
        fill(gridTemp, random->IRandom(0, dimRow), random->IRandom(0, dimCol), wallCount,
             total, floorCount, dimRow, dimCol);
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
        r=dimRow;
        do {
        //for(r=0; r < dimRow; ++r) {
            --r;
            c=dimCol;
            do {
                --c;
            //for(c=0; c < dimCol; ++c) {
                if(gridTemp[0][r][c]==0) {
                    grid[0][r][c].setWall();
                    ++wallCount;
                }
            }while(c);
        }while(r);

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
        for(r=0; r < boundariesRow; ++r) {
            delete[] gridTemp[l][r];
        }

        delete[] gridTemp[l];
    }

    delete[] gridTemp;

    return returnVal;
}

void cave::fill(int *** & gridTemp, long r, long c, unsigned short wallCount, unsigned short total,
                   unsigned short & floorCount, const unsigned short & dimRow,
                   const unsigned short & dimCol)
{
    if(gridTemp[0][r][c] == 0) {
        ++floorCount;
        gridTemp[0][r][c] = 1; //already touched

        if(r+1 < dimRow) {
            fill(gridTemp, r+1, c, wallCount, total, floorCount, dimRow, dimCol);
        }
        if(r-1 > 1) {
            fill(gridTemp, r-1, c, wallCount, total, floorCount, dimRow, dimCol);
        }
        if(c+1 < dimCol) {
            fill(gridTemp, r, c+1, wallCount, total, floorCount, dimRow, dimCol);
        }
        if(c-1 > 1) {
            fill(gridTemp, r, c-1, wallCount, total, floorCount, dimRow, dimCol);
        }
    }
}

void nullLocation::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullLocation object.");
}
