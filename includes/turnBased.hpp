#ifndef _TURNBASED_H
#define _TURNBASED_H

#pragma once
#include "lib.h"
#include "configuration.hpp"

class duration
//Turn-based lifetimes for objects
{
    public:
        duration();
        ~duration();

        /***********
          Setters
        ***********/

        void decrement();
        void set(long);
        inline unsigned int isActive() { return (turnsRemaining == 0) ? 0 : 1; } //if turnsRemaining >0 return 1 (true) else return 0 (false)
        inline unsigned int isPermenant() { return (turnsRemaining == OBJECT_LIFETIME_PERMANENT) ? 1 : 0; } //if turnsRemaining == -1 then return 1 (true) else return 0 (false)

        /***********
          Getters
        ***********/

        inline int get() const { return turnsRemaining; }

    protected:
        long turnsRemaining;
};

#endif
