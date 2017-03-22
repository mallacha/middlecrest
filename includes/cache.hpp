#ifndef _CACHE_H
#define _CACHE_H

#pragma once
#include "lib.h"

class ICache
//Interface for pool
{
    public:
        ICache();
        ~ICache();

        //Setters
        virtual void allocate(int); //Allocate memory for pool.

        //Getters
        inline int getSize() //Regular pool
            {
                return (int)cacheSize;
            }

         //virtual void get(unsigned int);

    private:
        unsigned short cacheSize; //Cache size (assumed pool starts at element zero)
};

#endif
