#ifndef _CHARACTER_H
#define _CHARACTER_H

#pragma once
#include "lib.h"
#include "configuration.hpp"
#include "object.hpp"

struct characterAttributes
//Used as an object, but really just a collection
{
    public:
        characterAttributes();
        ~characterAttributes();

        //Physical
        unsigned short strength; //physical strength
        unsigned short agility; //gross-motor skills
        unsigned short coordination; //fine-motor skills
        unsigned short endurance; //physical stamina

        //Spiritual
        unsigned short intuition; //wisdom
        unsigned short willpower; //mental stamina

        //Mental
        unsigned short intelligence; //higher-order mental capability
        unsigned short wits; //perception

        //Social
        unsigned short appearence; //attractiveness
        unsigned short charisma; //personal magnetism
};

struct characterLocation
{
    public:
        ~characterLocation();
        characterLocation();

        //World
        unsigned short worldX;
        unsigned short worldY;

        //Zone (zones are 20x20 areas within the world)
        unsigned short zoneX;
        unsigned short zoneY;

        //X, Y, Z
        unsigned short x;
        unsigned short y;
        unsigned short z;
};

class character
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
        void setXCoord(unsigned long);
        void setYCoord(unsigned long);
        void setZCoord(unsigned long);
        void save();
        void randomStartingLocation();

        /**********
           Getters
         **********/
        inline CharacterType getType() { return type; }
        inline unsigned short getXCoord() { return location.x; }
        inline unsigned short getYCoord() { return location.y; }
        inline unsigned short getZCoord() { return location.z; }
        inline char getSigil() { return sigil; }
        inline unsigned int getPlayerSaveId() { return save_id; }
        inline unsigned int getObjectId() { return object->getID(); }

        //Componants
        lifetime lifetimeObject;
        gameObject id;

    protected:
        gameObject * object;

        //Hot data
        unsigned int save_id; //Should be same size as game::player_instance
        CharacterType type; //PC or NPC
        characterLocation location;
        char sigil; //Character sigil
};

#endif
