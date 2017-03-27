#ifndef CHARACTERPOOLTEST_H
#define CHARACTERPOOLTEST_H

#pragma once
#include "../../includes/characterPool.hpp"

class characterPoolTest : public characterPool
{
    public:
        characterPoolTest() {}
        ~characterPoolTest() {}

        int allocate(int, int);
        void process() {}
        int setRandomStartingLocation();
};

#endif
