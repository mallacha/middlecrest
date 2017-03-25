#include "../includes/service.hpp"

using namespace std;

IMap::IMap()
{
    grid=NULL;
    dimLvl=0;
    generated=0;
}

IMap::~IMap()
{
    generated=0;
    dimLvl=0;
    //grid should be taken care of in dervied classes
}

cave::cave()
{
    generated=0;
    dimLvl=1; //levels
    grid = new tile**[dimLvl];
    IDisplay * display = locator::getDisplay();
    unsigned short dimRow = display->getDimY()+1;
    unsigned short dimCol = display->getDimX()+1;

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

    generate();
}

cave::~cave()
{
    IDisplay * display = locator::getDisplay();
    unsigned short dimRow = display->getDimY()+1;

    //Map deletion
    for(short l=0; l < dimLvl; ++l) {
        for(short x=0; x < dimRow; ++x) {
            delete[] grid[l][x];
        }

        delete[] grid[l];
    }

    delete[] grid;

    //Variables
    generated=0;
    dimRow=0;
    //dimCol=0;
    dimLvl=0;
    grid=NULL;
}

void cave::clearTiles()
{
    IDisplay * display = locator::getDisplay();
    unsigned short dimRow = display->getDimY();
    unsigned short dimCol = display->getDimX();

    for(short r=1; r < dimRow; ++r) {
        for(short c=1; c < dimCol; ++c) {
            grid[0][r][c].clear();
        }
    }
}

int cave::generate()
//Cave creation function
{
    unsigned short r=0;
    unsigned short c=0;
    short i=0;
    short count=0;
    short repeat=10; //Number of iterations: 10 = sea cave (something hollowed out by water, 2 = lava cave, 3 = ice/limestone cave, 1 = mud cave
    short wallFreq=40; //Frequency of wall
    IRand * random = locator::getRNG();
    IDisplay * display = locator::getDisplay();
    unsigned short dimRow = display->getDimY();
    unsigned short dimCol = display->getDimX();
    unsigned short boundariesRow = dimRow+1;
    unsigned short boundariesCol = dimCol+1;


#ifdef UNIT_TEST
//Testing this piece of code for segfaults
//and efficiency
mapGenerations=0;
timerSet(10); //in seconds
do {
#endif


    do {
        /*********************************************************************************
          Initialization: Steps through each tile and randomly assigns a wall tile. The
          location always starts completely empty.
        **********************************************************************************/
        clearTiles();

        for(r=0; r < boundariesRow; ++r) {
            for(c=0; c < boundariesCol; ++c) {
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
            for(r=1; r < dimRow; ++r) {
                for(c=1; c < dimCol; ++c) {
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
        for(r=1; r < dimRow; ++r) {
            for(c=1; c < dimCol; ++c) {
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

        for(r=0; r < boundariesRow; ++r) {
            for(c=0; c < boundariesCol; ++c) {
                if(c==0 || r==0 || c == dimCol || r == dimRow) {
                //Setting walls in bare location based on wall frequency
                    grid[0][r][c].setWall();
                }
            }
        }

            #ifdef UNIT_TEST
                ++mapGenerations;
            #endif

    }while(0 == floodFill(0.45));


#ifdef UNIT_TEST
}while(!timerFinished());
#endif


    //Set bool so not generated again
    generated = 1;

    return generated;
}

int cave::floodFill(float expectedRatio)
//Flood fills rooms with walls to fill in unconnected areas
{
    IRand * random = locator::getRNG();
    IDisplay * display = locator::getDisplay();
    unsigned short dimRow = display->getDimY();
    unsigned short dimCol = display->getDimX();
    unsigned short boundariesRow = dimRow+1;
    unsigned short boundariesCol = dimCol+1;
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
        for(c=0; c < boundariesCol; ++c) {
            for(r=0; r < boundariesRow; ++r) {
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
        for(c=0; c < dimCol; ++c) {
            for(r=0; r < dimRow; ++r) {
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
        for(r=0; r < boundariesRow; ++r) {
            delete[] gridTemp[l][r];
        }

        delete[] gridTemp[l];
    }

    delete[] gridTemp;

    return returnVal;
}

void cave::fill(int *** & gridTemp, long r, long c, unsigned short wallCount, unsigned short total,
                   unsigned short & floorCount, unsigned short dimRow, unsigned short dimCol)
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
