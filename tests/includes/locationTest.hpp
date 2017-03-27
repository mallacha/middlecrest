#ifndef LOCATIONTEST_H
#define LOCATIONTEST_H

#pragma once
#include "../../includes/location.hpp"

class IMapTest : public IMap
{
    public:
        IMapTest() {}
        ~IMapTest() {}

        long int generate();
};

#endif
