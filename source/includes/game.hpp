#ifndef _GAME_H
#define _GAME_H

#pragma once

#include "../../core/includes/lib.h"
#include "../../core/includes/logging.hpp"

class IGame
{
    public:
        IGame() {}
        ~IGame() {}

        virtual unsigned int getPcID() = 0;
        virtual char * getProvinceName() = 0;
        virtual void incrementTurn() = 0;
        virtual unsigned short getPCPoolElement() = 0;
        virtual unsigned short getZoneElement() = 0;
        virtual void setZoneElement(unsigned short) = 0;
        virtual unsigned long getTurn() = 0;
        virtual unsigned short getMapLocationX() = 0;
        virtual unsigned short getMapLocationY() = 0;
        virtual unsigned short getAreaLocationX() = 0;
        virtual unsigned short getAreaLocationY() = 0;
};

class game : public IGame
//Game state
{
    public:
        game();
        ~game();

        //Getters
        inline unsigned int getPcID() { return pcID; }
        inline unsigned long getTurn() { return turn; }
        inline unsigned short getMapLocationX() { return mapLocationX; }
        inline unsigned short getMapLocationY() { return mapLocationY; }
        inline unsigned short getAreaLocationX() { return areaLocationX; }
        inline unsigned short getAreaLocationY() { return areaLocationY; }
        inline char * getProvinceName() { return provinceName; }
        inline unsigned short getPCPoolElement() { return pcPoolElement; }
        inline unsigned short getZoneElement() { return zonePoolElement; }

        //Set
        void setProvinceName();
        void incrementTurn(); //TODO: Validate assumption that turn can overflow w/o problems
        inline void setZoneElement(unsigned short value) { zonePoolElement = value; }

    private:
        unsigned long turn;
        unsigned int pcID;
        char * provinceName;
        unsigned short mapLocationX; //Overmap location
        unsigned short mapLocationY; //Overmap location
        unsigned short areaLocationX; //Global location
        unsigned short areaLocationY; //Global location
        unsigned short pcPoolElement; //element of PC in character pool
        unsigned short zonePoolElement; //element of zone in location cache
};

class nullGame : public IGame
{
    public:
        nullGame();
        ~nullGame();

        inline unsigned int getPcID() { log(); }
        inline char * getProvinceName() { log(); }
        inline void incrementTurn() { log(); }
        inline unsigned short getPCPoolElement() { log(); }
        inline unsigned short getZoneElement() { log(); }
        inline void setZoneElement(unsigned short value) { log(); }
        inline unsigned long getTurn() { log(); }
        inline unsigned short getMapLocationX() { log(); }
        inline unsigned short getMapLocationY() { log(); }
        inline unsigned short getAreaLocationX() { log(); }
        inline unsigned short getAreaLocationY() { log(); }

    private:
        void log();
        ILogging * logger;
};

#endif
