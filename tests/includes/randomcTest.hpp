#ifndef RANDOMCTEST_H
#define RANDOMCTEST_H

#pragma once
#include "../../includes/randomc.hpp"

class TRandomMersenneTest : public IRand
{
    public:
        TRandomMersenneTest() {}
        ~TRandomMersenneTest() {}

        long IRandom(long, long);
        unsigned long RandomInit(long int);
        unsigned long BRandom() {}
        long int getSeed() {}
};

#endif
