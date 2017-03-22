#include "../includes/service.hpp"

using namespace std;

IPool::IPool()
{
    inactiveRegularIndex = -1;
    inactiveOverflowIndex = -1;
    poolSize=0;
    overflowSize=0;
}

IPool::~IPool()
{
    inactiveRegularIndex = 0;
    inactiveOverflowIndex = 0;
    poolSize=0;
    overflowSize=0;
}

int IPool::allocate(int pool, int overflow)
//Allocates pool and overflow pool
{
    if(setSize(pool, overflow)) {
    //If setting the size of the pool was successful

        //Assign head for regular pool
        inactiveRegularIndex = 0;

        //Assign head for overflow pool
        if (getOverflowSize()) {
            inactiveOverflowIndex = (int)getPoolSize();
        }
    }
}

int IPool::setSize(int pool, int overflow)
{
    //Validate that poolSize is positive
    if(pool <1) {
        ILogging * logger = locator::getLogging();
        if(overflow <1) {
            logger->debug("Primary pool allocated with zero elements. Amend code to rectify.");
        }else{
            logger->warn("Primary pool allocated with zero elements. Allocating overflow.");
        }
    }

    if (0 == poolSize) {
    //Only set size once

        //Set range
        pool = (unsigned short)validateUnsignedShort(pool);
        overflow = (unsigned short)validateUnsignedShort(overflow);

        //Set pool size and overflow index
        poolSize = pool;
        overflowSize = overflow;

        return poolSize + overflowSize;
    }

    //Didn't set size -- size already set
    return 0;
}
