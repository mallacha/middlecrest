#ifndef _CHARACTER_POOL_H
#define _CHARACTER_POOL_H

#pragma once
#include "lib.h"
#include "configuration.hpp"
#include "pool.hpp"
#include "character.hpp"
#include "logging.hpp"

/*Interface inherit an interface?
class ICharacterPool : public IPool
{
        //IPool inherited
        virtual int allocate(int, int) = 0;
        virtual void process() = 0;

        //Character pool testable functions
        vitrual int createCharacter(CharacterType) = 0;
        virutal void save() = 0;
};

class characterPool : public ICharacterPool
{
};
*/

class characterPool : public IPool
//Character pool
{
    public:
        characterPool();
        ~characterPool();

        void process(); //Process active objects

        //Getters
        character * getPool() { return poolArray; }
        inline long getLastActive() { return lastActive; }
        character * getCharacterByPoolId(unsigned int i) { return &poolArray[i]; }
        //character * getCharacterByObjectId(unsigned int i) {}
        unsigned int getObjectIdByPoolId(unsigned int index) { return poolArray[index].id.getID(); }
        unsigned int getSaveIdByPoolId(long index) { return poolArray[index].getPlayerSaveId(); }
        unsigned int getXCoordByPoolId(unsigned int);
        unsigned int getYCoordByPoolId(unsigned int);
        unsigned int getZCoordByPoolId(unsigned int);

        //Setters
        #ifdef UNIT_TESTS
        //For testing purposes
            virtual int allocate(unsigned short, unsigned short) = 0;
        #else
            int allocate(unsigned short, unsigned short);
        #endif
        int createCharacter(CharacterType);
        void save();
        void save(unsigned int index) { poolArray[index].save(); }
        int setXCoordByPoolId(unsigned int, unsigned int);
        int setYCoordByPoolId(unsigned int, unsigned int);
        int setZCoordByPoolId(unsigned int, unsigned int);
        int setMapIdByPoolId(unsigned int, unsigned int);
        #ifdef UNIT_TESTS
        //For testing purposes
            virtual void setRandomStartingLocation(unsigned int) = 0;
        #else
            void setRandomStartingLocation(unsigned int);
        #endif

    private:
        unsigned int lastActive; /* Stores last active element. Same type as pool.hpp->IPool::poolSize
                           Used for optimization during display. When displaying characters,
                           when the 'last' active element is hit then the function exits early.*/
        unsigned int ith; //'ith' stands for iterator (i-th). Same type as pool.hpp->IPool::poolSize
        character * poolArray;

        void checkForLastActiveElement();
        int createTable(); //Create database table for character pool
};

class nullCharacter : public characterPool
//Null location
{
    public:
        nullCharacter() { logger=NULL; }
        ~nullCharacter() { logger=NULL; }

        //Interface functions
        int allocate() { log(); return 0; }
        void process() { log(); }
        void createCharacter(CharacterType type) { log(); }

    private:
        void log();

        ILogging * logger;
};

#endif
