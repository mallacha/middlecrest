#ifndef DISPLAYTEST_H
#define DISPLAYTEST_H

#pragma once
#include "../../includes/display.hpp"

class IDisplayTest : public IDisplay
{
    public:
        IDisplayTest() {}
        ~IDisplayTest() {}

        virtual void draw() {}
        virtual void create() {}
        virtual unsigned int getDimX() { return 0; }
        virtual unsigned int getDimY() { return 0; }
        int randomStartingLocation();
};

#endif
