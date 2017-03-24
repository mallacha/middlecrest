#include "../includes/characterPool.hpp"
#include "../includes/service.hpp"
#include "../includes/command.hpp"

using namespace std;

/**********************************************************
                     Helper Functions
***********************************************************/

unsigned int checkOverflowPoolFirst(CharacterType cType)
{
    if(PC == cType) {
        return 1;
    }

    return 0;
}

/**********************************************************
                          Objects
***********************************************************/

characterPool::characterPool()
{
    poolArray=NULL;
    lastActive=0;
    ith=0;
}

characterPool::~characterPool()
{
    lastActive=0;
    ith=0;
    if(poolArray) {
        delete [] poolArray;
    }
}

unsigned int characterPool::getXCoordByPoolId(unsigned int i)
{
    if(i < getSize()) {
    //Checking bounds
        return poolArray[i].getXCoord();
    }

    return 0;
}

unsigned int characterPool::getYCoordByPoolId(unsigned int i)
{
    if(i < getSize()) {
    //Checking bounds
        return poolArray[i].getYCoord();
    }

    return 0;
}

unsigned int characterPool::getZCoordByPoolId(unsigned int i)
{
    if(i < getSize()) {
    //Checking bounds
        return poolArray[i].getZCoord();
    }

    return 0;
}

int characterPool::setXCoordByPoolId(unsigned int i, unsigned int x)
{
    if(i < getSize()) {
    //Checking bounds
        poolArray[i].setXCoord(x);
        return 1; //true
    }

    return 0; //false
}

int characterPool::setYCoordByPoolId(unsigned int i, unsigned int y)
{
    if(i < getSize()) {
    //Checking bounds
        poolArray[i].setYCoord(y);
        return 1; //true
    }

    return 0; //false
}

int characterPool::setZCoordByPoolId(unsigned int i, unsigned int z)
{
    if(i < getSize()) {
    //Checking bounds
        poolArray[i].setZCoord(z);
        return 1; //true
    }

    return 0; //false
}

int characterPool::allocate(unsigned short pool, unsigned short overflow)
//Allocates pool and overflow pool
{
    int status=0;

    status = IPool::allocate(pool, overflow);

    if(getSize() && !poolArray) {
        poolArray = new character[getSize()];

            #ifdef DEBUG
                ILogging * logger = locator::getLogging();
                logger->trace("Allocating character pool(size %d, address %d)", getSize(), &poolArray[0]);
            #endif
    }else{
        status=1; //Error state. status==0 if everything is okay.
                  //This is changing the value to a not okay value
    }

    //Create character table
    createTable();

    return status;
}

int characterPool::createCharacter(CharacterType cType)
{
    int created=0;

    //A test to see if the pool has been allocated. Skip code if it hasn't
    if(getSize()) {

        unsigned int totalPoolSize=getSize(); //allocate a variable to keep track of boundaries
        ith = getPoolSize(); //Overflow starts at end of regular pool

        //Does pool have an overflow? 'i' will be less than total pool size
        //Also only check the overflow pool if character type needs to be added
        //to pool no matter what (player characters, bosses, etc.)
        if(ith+1 <= totalPoolSize && checkOverflowPoolFirst(cType)) {

            IGame * game = locator::getGame();
            //Iterate until we create a character or we hit the pool boundary
            do {
                //If element is not filled with an active character
                if(!poolArray[ith].lifetimeObject.isActive()) {

                    //Create character
                    poolArray[ith].setType(cType);

                    if(ith > lastActive) {
                    //Update last active element indicator
                        lastActive = ith;
                    }

                    //Save character immediately to database if we are creating
                    //a player character (PC) and the game object isn't updated
                    //yet
                    if(!game->getPlayerInstance()) {
                        poolArray[ith].save(); //So we have a copy in database
                        game->setPlayerInstance(poolArray[ith].getPlayerSaveId());
                    }

                    return ith; //return object pool element so it can be referenced
                                //later
                }

                ++ith;
            }while(!created && ith < totalPoolSize);
        }

        //If character hasn't been created yet, iterate through regular pool
        if(!created) {

            totalPoolSize = getPoolSize(); //reset totalPoolSize so we only scan regular pool
            ith=0;

            do {
                //If element is not filled with an active character
                if(!poolArray[ith].lifetimeObject.isActive()) {
                    //Create character
                    /*
                      TODO: put in NPC code
                    */
                    poolArray[ith].setType(cType);
                    poolArray[ith].save();
                    created=1;
                }

                ++ith;
            }while(!created && ith < totalPoolSize);
        }

        //Clean up. Reset last active element indicator
        checkForLastActiveElement();
    }
    #ifdef DEBUG
        else {
            ILogging * logger = locator::getLogging();
            logger->attn("Character pool tried to create a character but has not been allocated. Allocate first?");
        }

        created=-1; //not created -- unallocated pool
    #endif

    #ifdef BETA
        //Log if we hit the end of the pool and couldn't create an object. This
        //is important information for tuning the pool.
        if(!created) {
            ILogging * logger = locator::getLogging();
            #ifdef DEBUG
                logger->error("Program attempted to create a new character but the character pool is full (%d objects). Program may exhibit undefined behavior or have erroneous states. Please consider tuning the character pool by increasing the pool or limiting the lifetime of game objects", getSize());
            #else
                logger->warn("Program attempted to create a new character but the character pool is full (%d objects). Program may exhibit undefined behavior or have erroneous states. Please consider tuning the character pool by increasing the pool or limiting the lifetime of game objects", getSize());
            #endif
        }

        created=-2; //not created -- end of pool
    #endif

    //TODO: returns 'created' so that unit tests can see if an object was
    //created. Unit tests should use this variable to detect if a character was
    //NOT CREATED (created < 1) and fail.
    //CREATED (created > 0)
    return created;
}

void characterPool::save()
{
    unsigned int totalPoolSize = getSize(); //scan regular pool

    //A test to see if the pool has been allocated. Skip code if it hasn't
    if(totalPoolSize) {
            ith=0;

            do {
                poolArray[ith].save();

                ++ith;
            }while(ith < totalPoolSize);
    }
}

void characterPool::process()
//Process active objects
{
    unsigned size = getSize();
    duration * lifetimeObj;

    for(unsigned int i = 0; i < size; ++i) {

        lifetimeObj = poolArray[i].lifetimeObject.getLifetimeObject();
        lifetimeObj->decrement();

        //TODO: probably shouldn't do this here, but is convenient to get player input
        if(poolArray[i].getType() == PC) {
            handleInput(&poolArray[i], i);
        }
    }
}

void characterPool::checkForLastActiveElement()
//Checks for last active element. Used as an efficiency tool to stop displaying
//when we have reached the last active character.
{
    unsigned int totalPoolSize=getSize(); //allocate a variable to keep track of boundaries
    ith = lastActive;

    do {
        if(poolArray[ith].lifetimeObject.isActive()) {
            lastActive = ith;
        }

        ++ith;
    }while(ith < totalPoolSize);

        #ifdef ASSERTION_TEST

            assert(ith == totalPoolSize);

            //last active element indicator should never be equal to or larger
            //than total pool size. If it were, that would mean that it is
            //out of bounds and could case an error or undefined behavior
            assert(lastActive < totalPoolSize);
        #endif
}

int characterPool::createTable()
{
    return saveDataTableCreation(
                            "SELECT name FROM sqlite_master WHERE type='table' AND name='characters' LIMIT 1;",
                            81,
                            "CREATE TABLE characters (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT DEFAULT 'Yochanan Dhehy', player_character_bool INTEGER DEFAULT 0 NOT NULL, mid INTEGER, FOREIGN KEY(mid) REFERENCES maps(mid));",
                            250);
}

void nullCharacter::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullCharacter object.");
}
