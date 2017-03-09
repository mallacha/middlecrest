#ifndef _DISPLAY_H
#define _DISPLAY_H

#pragma once
#include "../../core/includes/lib.h"
#include "../../core/includes/display.hpp"
#include "../../core/includes/logging.hpp"
#include <curses.h>

class asciiDisplay : public IDisplay
{
    public:
        asciiDisplay();
        ~asciiDisplay();

        void create();
        void draw();

    private:
        WINDOW * my_map; //Playing map (sometimes doubles as menu window)
        WINDOW * my_loc; //Location banner/window
};

class nullDisplay : public IDisplay
//Null location
{
    public:
        nullDisplay() { logger=NULL; }
        ~nullDisplay() {}

       //Interface functions
        inline void draw() { log(); }

    private:
        void log();

        ILogging * logger;
};

#endif
