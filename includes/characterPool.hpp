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

        //Getters
        character * getPool() { return poolArray; }
        inline long getLastActive() { return lastActive; }
        character * getCharacterByPoolId(unsigned int i) { return &poolArray[i]; }
        //character * getCharacterByObjectId(unsigned int i) {}
        unsigned int getCharacterIdByPoolId(unsigned int i) { return poolArray[i].id.getID(); }

        unsigned int getXCoordByPoolId(unsigned int i);
        unsigned int getYCoordByPoolId(unsigned int i);
        unsigned int getZCoordByPoolId(unsigned int i);

        //Setters
        int allocate(unsigned short, unsigned short); //Allocate memory for pool.
        int createCharacter(CharacterType);
        void save();

        void process(); //Process active objects

        int setXCoordByPoolId(unsigned int i, unsigned int x);
        int setYCoordByPoolId(unsigned int i, unsigned int y);
        int setZCoordByPoolId(unsigned int i, unsigned int z);

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
