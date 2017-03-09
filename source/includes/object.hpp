#ifndef _OBJECT_H
#define _OBJECT_H

#pragma once
#include "../../core/includes/object.hpp"

class lifetime : public ILifetime
//Object lifetime component. Adding methods only for game code.
{
    public:
        lifetime();
        ~lifetime();

        /***********
          Setters
        ***********/

        /***********
          Getters
        ***********/

        inline int isActive() const { return (duration == 0) ? 0 : 1; } //if lifetime >0 return 1 else return 0
        //Duration of object: -1=permanent (active), 0=inactive, >1=temporary (active)
};

#endif
