#ifndef _POOL_H
#define _POOL_H

#pragma once
#include "lib.h"
//#include "object.hpp"

class IPool
//Interface for pool
{
    public:
        IPool();
        ~IPool();

        //Setters
        virtual int allocate(int, int); //Allocate memory for pool.
        virtual void process() {} //Process pool

        //Getters
        inline unsigned int getPoolSize() //Regular pool
            {
                return (unsigned int)poolSize;
            }
        inline unsigned int getOverflowSize() //Overflow pool
            {
                return (unsigned int)overflowSize;
            }
        inline unsigned int getSize() //Combined total size (regular + overflow)
            {
                return (unsigned int)(poolSize + overflowSize);
            }

    protected:
        int setSize(int, int); //Set size for pool and overflow

        int inactiveRegularIndex;
        int inactiveOverflowIndex;

    private:
        unsigned short poolSize; //Size of pool (assumed pool starts at element zero)
        unsigned short overflowSize; //Size of overflow (assumed overflow starts at poolSize+1)
};

#endif
