#include "../includes/pool.hpp"
#include "../includes/service.hpp"
#include "../includes/configuration.hpp"

using namespace std;

/**
    Pool Base Class
**/

characterPool::characterPool()
{
    poolArray = NULL;
    totalObjects = 0;
}

characterPool::~characterPool()
{
    delete [] poolArray;
    totalObjects = 0;
}

void characterPool::allocate(unsigned short pool, unsigned short overflow)
//Allocates pool and overflow pool
{
    if(setSize(pool, overflow)) {
    //If setting the size of the pool was successful

        //Allocate regular array
        poolArray = new character[getSize()];

            #ifdef DEBUG
                ILogging * logger = locator::getLogging();
                logger->trace("Allocating object pool(size %d, address %d)", getSize(), &poolArray[0]);
            #endif

        //Assign head for regular pool
        inactiveRegularIndex = 0;

        //Assign head for overflow pool
        if (getOverflowSize()) {
            inactiveOverflowIndex = (int)getPoolSize();
        }
    }
}

void characterPool::createStatic()
//Create permanant object
{
    if(inactiveRegularIndex != -1) {
        poolArray[inactiveRegularIndex].setPermanent();
    } else if (inactiveOverflowIndex != -1) {
        poolArray[inactiveOverflowIndex].setPermanent();
    }

    //Find next available object for each pool
    findOverflowReplacement();
    findRegularReplacement();
}

void characterPool::createStaticPC()
//Create player-character permanant object
{
    if(inactiveRegularIndex != -1) {
        poolArray[inactiveRegularIndex].initialize();
        poolArray[inactiveRegularIndex].setType(PC);
    } else if (inactiveOverflowIndex != -1) {
        poolArray[inactiveOverflowIndex].initialize();
        poolArray[inactiveOverflowIndex].setType(PC);
    }

    //Find next available object for each pool
    findRegularReplacement();
    findOverflowReplacement();
}

void characterPool::createObject()
//Create temporary object
{
    if(inactiveRegularIndex != -1) {
        poolArray[inactiveRegularIndex].setTemporary();
    }

    findRegularReplacement();
}

void characterPool::createPC()
//Create player character in pool
//TODO: This is the first pass of the function, will
//      change as code advances.
{
    character * object = NULL;
    unsigned int size = getSize(); //Get size of pool + overflow
    unsigned short created=0;
    unsigned short i=0; //Same type as core::pool.hpp->IPool::poolSize

    do {
        object = &(poolArray[i]);
        if(object && object->getType() == PC) {
            created = 1;
        }
        ++i;
    }while(0 == created && i < size);

    if(0 == created) {
        createStaticPC();
    }
        #ifdef DEBUG
            else {
                ILogging * logger = locator::getLogging();
                logger->trace("Player-character object already created");
            }
        #endif
}

void characterPool::process()
//Process active objects
{
    unsigned size = getSize();

    for(unsigned int i = 0; i < size; ++i) {
        //printf("Regular: %p, Type: %d, Life: %d, ID: %d Objects:  %d Pool: %d  i: %d\n", &poolArray[i], poolArray[i].getType(),
        //poolArray[i].objectLifetime.getLifetime(), poolArray[i].getID(), totalObjects, size, i);

        poolArray[i].objectLifetime.decrementLifetime();

        //TODO: probably shouldn't do this here, but is convenient to get player input
        if(poolArray[i].getType() == PC) {
            handleInput(&poolArray[i], i);
        }
    }
}

void characterPool::findRegularReplacement()
//Finds next available pool index
{
    unsigned int oldestLifetime=OBJECT_LIFETIME_LONG;
    int lifetime=OBJECT_LIFETIME_LONG;
    unsigned short found=0;

    //Find oldest object
    for(unsigned int i=0; i < getPoolSize(); ++i) {
        lifetime = poolArray[i].objectLifetime.getLifetime();
        if(lifetime > -1 && lifetime < oldestLifetime) {
            inactiveRegularIndex=i;
            oldestLifetime = lifetime;

            found=1;
        }

        if(i >= totalObjects && poolArray[i].objectLifetime.isActive()) {
        //Last active object in pool?
            totalObjects = i+1;
        }
    }

    if(0 == found) {
        inactiveRegularIndex=-1; //indicates no pool index can be used
    }
}

void characterPool::findOverflowReplacement()
//Finds next available overflow index
{
    unsigned int oldestLifetime=OBJECT_LIFETIME_LONG;
    int lifetime=OBJECT_LIFETIME_LONG;
    unsigned short found=0;

    //Find oldest object
    for(unsigned int i=getPoolSize(); i < getSize(); ++i) {
        lifetime = poolArray[i].objectLifetime.getLifetime();
        if(lifetime > -1 && lifetime < oldestLifetime) {
            inactiveOverflowIndex=i;
            oldestLifetime = lifetime;

            found=1;
        }

        if(i >= totalObjects && poolArray[i].objectLifetime.isActive()) {
        //Last active object in pool?
            totalObjects = i+1;
        }
    }

    if(0 == found) {
        inactiveOverflowIndex=-1; //indicates no pool index can be used
    }
}


void nullCharacter::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullCharacter object.");
}
