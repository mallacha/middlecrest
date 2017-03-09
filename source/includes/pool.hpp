#ifndef _POOL_H
#define _POOL_H

#pragma once
#include "../../core/includes/lib.h"
#include "../../core/includes/pool.hpp"
#include "../../core/includes/logging.hpp"
#include "command.hpp"

class characterPool : public IPool
//Pool
//Common code used for pool whose objects have a lifetime. The code itself
//is agnostic to the underlying object (it could be a character, item, etc.)
//Children derived and pool's parents are concerned with allocating
//particular types of object. This pool class deals with the common algorigthms
//used to allocate pools and arrange pointers for the active and inactive lists.
{
    public:
        characterPool();
        ~characterPool();

        /*Pool creation functions
            Normal steps of creating a pool object:
            allocate(number, number);
            create();

            Afterwards, when you want to create an object:
            createObject();
        */

        //Setters
        void allocate(unsigned short, unsigned short); //Allocates pool and overflow.
        void createStatic(); //Create permanent object
        void createStaticPC(); //Create player-character
        void createObject(); //Create temporary object

        virtual void createPC(); //Some redundant code with createStaticPC

        virtual void process(); //Process active objects
        void setZoneID(unsigned short element, unsigned long value) { poolArray[element].setZoneID(value); }


        //Getters
        virtual character * getPool() { return poolArray; }
        unsigned long getTotalObjects() { return totalObjects; }

    protected:
        void findRegularReplacement();
        void findOverflowReplacement();

        character * poolArray;
        unsigned long totalObjects;
};

class nullCharacter : public characterPool
//Null location
{
    public:
        nullCharacter() { logger=NULL; }
        ~nullCharacter() { logger=NULL; }

        //Interface functions
        int create() { log(); }
        void process() { log(); }
        void createPC() { log(); }
        character * getPool() { log(); }

    private:
        void log();

        ILogging * logger;
};

#endif
