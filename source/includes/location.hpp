#ifndef _LOCATION_H
#define _LOCATION_H

#pragma once
#include "../../core/includes/location.hpp"
#include "../../core/includes/logging.hpp"
#include "../../core/includes/pool.hpp"
#include "../../core/includes/logging.hpp"
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

class zone
//Zone object: X by Y tiles. 
//This is the screen that characters move around in.
{
    public:
        zone();
        ~zone();

        virtual int generate() = 0; //Generate zone

        //Getters
        inline int getDimRow() { return dimRow-1; }
        inline int getDimCol() { return dimCol-1; }
        inline int getDimLvl() { return dimLvl; }
        tile *** getGrid() { return grid; }
        void clearTiles();

        //Setters
        void setSeed(unsigned long value) { seed = value; }

    protected:
        tile *** grid;
        unsigned long seed;
        unsigned short dimLvl; //levels
        unsigned short dimRow; //rows
        unsigned short dimCol; //columns
};

class zoneCacheObject : public IObject
{
    public:
        zoneCacheObject();
        ~zoneCacheObject();

        //Getters
        inline unsigned long getID() { return id; }
        inline unsigned long getSeed() { return seed; }
        inline unsigned long getLastAccess() { return lastAccess; }
        inline unsigned short getMapLocationX() { return mapLocationX; }
        inline unsigned short getMapLocationY() { return mapLocationY; }
        inline unsigned short getAreaLocationX() { return areaLocationX; }
        inline unsigned short getAreaLocationY() { return areaLocationY; }

        //Setters
        inline void setID(unsigned long value) { id = value; }
        void setLastAccess(); //Call world object. Set lastAccess to turn
        inline void setMapLocationX(unsigned short value) { mapLocationX = value; }
        inline void setMapLocationY(unsigned short value) { mapLocationY = value; }
        inline void setAreaLocationX(unsigned short value) { areaLocationX = value; }
        inline void setAreaLocationY(unsigned short value) { areaLocationY = value; }
        inline void setSeed(unsigned long value) { seed = value; }

    private:
        unsigned long id; //zone id
        unsigned long lastAccess; //based on turn
        unsigned long seed;
        unsigned short mapLocationX; //Overmap location
        unsigned short mapLocationY; //Overmap location
        unsigned short areaLocationX; //Global location
        unsigned short areaLocationY; //Global location
};

class IZoneCache
{
    public:
        virtual zone * getZone() = 0;
};

class zoneCache : public IZoneCache
//Caches zones previously visited by player for quicker retrieval
{
    public:
        zoneCache();
        ~zoneCache();

        //Setters
        zone * getZone();
        void insertZone();

    private:
        void addZone() {}

        vector<zoneCacheObject *> zones;
        zone * locationObj; //Operates as a flyweight
};

class nullZoneCache : public IZoneCache
{
    public:
          nullZoneCache() {}
          ~nullZoneCache() {}

          inline zone * getZone() { log(); }

    private:
        void log();

        ILogging * logger;
};

class cave : public zone
//Cave: zone subtype
{
    public:
        cave() {}
        ~cave() {}

        int generate();

        int floodFill(float);
        int fill(int *** &, long, long, unsigned short, unsigned short, unsigned short &); //Recursively fill a tile
};

/*******************************************************

  TODO: Following block of code needs to be removed.
        Using the zone object is sufficient.

*******************************************************/

/*class MCLocation : public ILocation
//Interface for location
{
    public:
        virtual zone * getZone() = 0; //Get zone
};

class location : public MCLocation
//Location object
{
    public:
        location() { grid=NULL; }
        ~location();

        //Interface functions
        void allocate(); //Create object
        void initialize(); //Generate zone

        //Getters
        inline zone * getZone() { return grid; }

    protected:
        zone * grid; //polymorphic

        //Terminology:
        //tile = tile
        //zone = X by Y tiles
        //area = collection of zones (overmap uses 24 by 24 zones?)
        //map = collection of areas (overmap is 1 map)
};

class nullLocation : public MCLocation
//Null location
{
    public:
        nullLocation() { logger=NULL; }
        ~nullLocation() { logger=NULL; }

        //Interface functions
        void allocate() { log(); }
        void initialize() { log(); }
        zone * getZone() { log(); }

    private:
        void log();

        ILogging * logger;
};

/*******************************************************

  End zone

*******************************************************/

#endif
