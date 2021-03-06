#ifndef _ASCII_H
#define _ASCII_H

#pragma once
#include "lib.h"
#include "location.hpp"

class IDisplay
//Ascii interface
{
    public:
        IDisplay();
        virtual ~IDisplay();

        virtual void draw() = 0;
        virtual void create() = 0;
        virtual int isImpassible(unsigned short, unsigned short, unsigned short) = 0;
        unsigned int createGameMap(LocationType);
        void linkMaps(unsigned int, unsigned int);
        int generate() { return map->generate(); }

        //Setters
        virtual unsigned int getDimX() = 0;
        virtual unsigned int getDimY() = 0;
        void set(unsigned int);

        //Getters
        IMap * getMap() { return map; }
        int * getRandomStartingLocation();

    protected:

        IMap * map;
        int random_xy[2];
};

class ascii : public IDisplay
{
    public:
        ascii();
        ~ascii();

        void create();
        void draw();
        inline unsigned int getDimX() { return (unsigned int)(dimCol-1); }
        inline unsigned int getDimY() { return (unsigned int)(dimRow-1); }
        inline int isImpassible(unsigned short z, unsigned short x, unsigned short y)
                                 { return map->isImpassible(z, x, y); }

    private:
        WINDOW * my_map; //Playing map (sometimes doubles as menu window)
        WINDOW * my_loc; //Location banner/window
        unsigned short dimCol; //columns
        unsigned short dimRow; //rows
};

class nullDisplay : public IDisplay
//Null location
{
    public:
        nullDisplay() { logger=NULL; }
        ~nullDisplay() {}

       //Interface functions
        inline void draw() { log(); }
        inline void create() { log(); }
        inline unsigned int getDimX() { log(); return 0; }
        inline unsigned int getDimY() { log(); return 0; }
        int isImpassible(unsigned short z , unsigned short x, unsigned short y) { log(); return 0; }

    private:
        void log();

        ILogging * logger;
};

#endif
