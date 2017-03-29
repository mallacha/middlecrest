#ifndef DISPLAYTEST_H
#define DISPLAYTEST_H

#pragma once
#include "../../includes/ascii.hpp"

class IDisplayTest : public IDisplay
{
    public:
        IDisplayTest() {}
        ~IDisplayTest() {}

        void draw() {}
        void create() {}
        unsigned int getDimX() { return 0; }
        unsigned int getDimY() { return 0; }
        int isImpassible(unsigned short a, unsigned short b, unsigned short c) {}
        int randomStartingLocation();
};

#endif
