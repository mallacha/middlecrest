#ifndef _LOCATION_H
#define _LOCATION_H

#pragma once
#include "logging.hpp"
#include "configuration.hpp"

class tile
//Tile object
{
    public:
        tile() { features=NO_FEATURE; attributes=NO_ATTRIBUTE; type=0; }
        ~tile() {  features=NO_FEATURE; attributes=NO_ATTRIBUTE; type=0; }

        //Getters
        inline int isAttribute(unsigned int val)      { return attributes & val; }
        inline int isFeature(unsigned int val)        { return features & val; }
        inline int isType(unsigned int val)           { return type==val; }

        //Setters
        inline void setFeature(unsigned int val)      { features=val; }
        inline void setAttribute(unsigned int val)    { attributes=val; }
        inline void addAttribute(unsigned int val)    { attributes |= val; }
        inline void removeAttribute(unsigned int val) { attributes &= ~val; }
        inline void setType(unsigned int val)         { type=val; }
        inline void clear()                           { features=0; attributes=0; type=0; }

        //Helper (setters)
        inline void setWall()                         { features = WALL_FEATURE; attributes |= IMPASSIBLE_ATTRIBUTE; }

        //Helper (getters)
        inline int isWall()                           { return features & WALL_FEATURE; }

    private:
        unsigned int features; //Features (as seen by player)
        unsigned int attributes; //Feature attributes
        unsigned int type; //Feature type
};

class IMap
//X by Y tiles
{
    public:
        IMap();
        virtual ~IMap();

        virtual long generate() { return 0; }

        //Setters
        inline void setMapId(unsigned int value) { map_id = value; }
        inline void setSeed(long value) { seed = value; }

        //Getters
        inline int getDimZ() { return dimLvl; }
        tile *** getGrid() { return grid; }
        inline int isImpassible(unsigned short z, unsigned short x, unsigned short y)
                                 { return grid[z][y][x].isAttribute(IMPASSIBLE_ATTRIBUTE); }

    protected:
        tile *** grid;
        long seed;
        unsigned int map_id; //bool: 0 = false, 1 = true
        unsigned short dimLvl; //levels
};

class cave : public IMap
//Cave: zone subtype
{
    public:
        cave() {}
        ~cave() {}

        //Setters
        long generate(); //generate random map (does not save)

    private:
        inline int floodFill(const float);
        static void fill(int *** &, long, long, unsigned short, unsigned short &,
                 const unsigned short &, const unsigned short &); //Recursively fill a tile
};

class nullLocation : public IMap
//Null location
{
    public:
        nullLocation() { logger=NULL; }
        ~nullLocation() { logger=NULL; }

        //Interface functions
        long generate() { log(); return 0; }

    private:
        void log();

        ILogging * logger;
};

#endif
