#ifndef POOLTEST_H
#define POOLTEST_H

#pragma once
#include "../../includes/pool.hpp"

class IPoolTest : public IPool
{
    public:
        IPoolTest() {}
        ~IPoolTest() {}

        int allocate(int, int);
        void process() {}
};

#endif
