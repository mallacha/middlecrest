#include "../includes/service.hpp"

using namespace std;

IMap::IMap()
{
    dimLvl=1;
    map_id=0;
    seed=0;
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
    grid=NULL;
    map_id=0;
    dimLvl=0;
    seed=0;
}

long cave::generate()
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
    random->RandomInit(seed);
    const unsigned short dimRow = display->getDimY();
    const unsigned short dimCol = display->getDimX();
    const unsigned short boundariesRow = dimRow+1;
    const unsigned short boundariesCol = dimCol+1;

    do {
        /*********************************************************************************
          Initialization: Steps through each tile, clears it, and randomly assigns a wall
          tile.
        **********************************************************************************/

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

        //Draw boundaries
        r=boundariesRow;
        do {
        //for(r=0; r < boundariesRow; ++r) {
            --r;
            grid[0][r][0].setWall();
            grid[0][r][dimCol].setWall();
        }while(r);
        c=boundariesCol;
        do {
        //for(c=0; c < boundariesCol; ++c) {
            --c;
            grid[0][0][c].setWall();
            grid[0][dimRow][c].setWall();
        }while(c);

    }while(0 == floodFill(0.45));

    return seed; //doubles as a success (true > 0)
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

    unsigned short floorCount=0;
    unsigned short attempts=0;
    const unsigned short maxAttempts=5;
    float ratio = (float)floorCount / (float)total;

/************************************************************************
    Main loop. Continues to flood fill until empty tile count is
    greater than the exprected ratio and attempts are less than the
    maximum. If the maximum attempts is exceeded, the grid may not be
    the best fit or the randomly selected points may not be choosing
    a satisfactorily connected tile. Another grid will be generated.
************************************************************************/

    while(ratio < expectedRatio && attempts < maxAttempts) {
        //Initialize gridTemp. Set walls and empty tiles.
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
                }else{
                    gridTemp[0][r][c]=0;
                }
            }while(c);
        }while(r);

        //Fill empty space. Calculate ratio. Keep track of attempts.
        floorCount=0;
        fill(gridTemp, random->IRandom(0, dimRow), random->IRandom(0, dimCol), total,
             floorCount, dimRow, dimCol);
        ratio = (float)floorCount / (float)total;

        if(floorCount > 0) {
        //Only increase counter if we counted something.
        //Sometimes the random square we check is filled.
            ++attempts;
        }
    }

/************************************************************************
    Determine whether it was a good generation. If maxAttempts exceeded
    then it wasn't. Assign returnVal accordingly.
************************************************************************/

    unsigned short result = 1; //success
    if(attempts > maxAttempts-1) {
    //Bad generation if we go over attempts
        result=0; //failure
    }

    if(result == 1) {
    //Patch
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
                }
            }while(c);
        }while(r);
    }

/************************************************************************
    Delete tempGrid and return success or failure
************************************************************************/

    for(unsigned short l=0; l < dimLvl; ++l) {
        for(r=0; r < boundariesRow; ++r) {
            delete[] gridTemp[l][r];
        }

        delete[] gridTemp[l];
    }

    delete[] gridTemp;

    return result;
}

void cave::fill(int *** & gridTemp, long r, long c, unsigned short total, unsigned short & floorCount,
                const unsigned short & dimRow, const unsigned short & dimCol)
{
    if(gridTemp[0][r][c] == 0) {
        ++floorCount;
        gridTemp[0][r][c] = 1; //already touched

        if(r+1 < dimRow) {
            fill(gridTemp, r+1, c, total, floorCount, dimRow, dimCol);
        }
        if(r-1 > 1) {
            fill(gridTemp, r-1, c, total, floorCount, dimRow, dimCol);
        }
        if(c+1 < dimCol) {
            fill(gridTemp, r, c+1, total, floorCount, dimRow, dimCol);
        }
        if(c-1 > 1) {
            fill(gridTemp, r, c-1, total, floorCount, dimRow, dimCol);
        }
    }
}

void nullLocation::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullLocation object.");
}
