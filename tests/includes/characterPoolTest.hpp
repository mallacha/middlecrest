#ifndef CHARACTERPOOLTEST_H
#define CHARACTERPOOLTEST_H

#pragma once
#include "../../includes/characterPool.hpp"

class ICharacterPoolTest : public characterPool
{
    public:
        ICharacterPoolTest() {}
        ~ICharacterPoolTest() {}

        int allocate(int, int);
        void process() {}
};

#endif
