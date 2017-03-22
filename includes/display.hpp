#ifndef _DISPLAY_H
#define _DISPLAY_H

#pragma once
#include "lib.h"

class IDisplayFormat
//Display format interface
{
    public:
        virtual void draw() = 0;
        virtual void create() = 0;
};

#endif
