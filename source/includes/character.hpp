#ifndef _CHARACTER_H
#define _CHARACTER_H

#pragma once
#include "../../core/includes/lib.h"
#include "../../core/includes/logging.hpp"
#include "object.hpp"
#include "configuration.hpp"

class character : public entity
//Character object
{
    public:
        character();
        ~character();

        /**********
           Setters
         **********/
        void setType(CharacterType); //Character type (PC or NPC)
        void setPermanent(); //Sets permanent object
        void setTemporary(); //Sets temporary object
        void setZoneID(unsigned long value) { zoneID=value; }

        //Actions
        void moveCharacter(unsigned short, unsigned short, unsigned short);

        /**********
           Getters
         **********/
        inline CharacterType getType() { return type; }
        inline unsigned short getXCoord() { return x; }
        inline unsigned short getYCoord() { return y; }
        inline unsigned short getZCoord() {return z; }
        inline char getSigil() { return sigil; }
        inline unsigned int getZoneID() { return zoneID; }

        inline int getLifetime() { objectLifetime.getLifetime(); }

        //Componants
        lifetime objectLifetime;

    protected:
        //Hot data
        CharacterType type; //PC or NPC
        unsigned long zoneID; //ID of zone character is currently located
        unsigned short x; //x-cooridnate on screen
        unsigned short y; //y-coordinate on screen
        unsigned short z; //z-coordinate on screen
        char sigil; //Character sigil
};

#endif
