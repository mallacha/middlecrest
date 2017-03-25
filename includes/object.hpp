#ifndef _OBJECT_H
#define _OBJECT_H

#pragma once
#include "lib.h"
#include "turnBased.hpp"

class gameObject
{
    public:
        gameObject();
        ~gameObject();

        inline long getID() { return id; }

    private:
        unsigned int id;
        static unsigned int next_id;
};

class lifetime
//Object lifetime
{
    public:
        lifetime() {}
        ~lifetime() {}

        /***********
          Setters
        ***********/

        inline void reduceLifetime() { lifetimeObject.decrement(); }
        inline int isActive() { return lifetimeObject.isActive(); }
        inline int isPermenant() { return lifetimeObject.isPermenant(); }

        /***********
          Getters
        ***********/

        inline duration * getLifetimeObject() { return &lifetimeObject; }

    private:
        duration lifetimeObject;
};

#endif
