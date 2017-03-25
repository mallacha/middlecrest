/*****************************************
 Global configuration
*****************************************/

#ifndef _CONFIG_H
#define _CONFIG_H

#pragma once
#include "lib.h"

#define OBJECT_LIFETIME_PERMANENT -1
#define OBJECT_LIFETIME_LONG 2000
#define OBJECT_LIFETIME_REDUCED 1000
#define OBJECT_LIFETIME_SHORT 500
#define OBJECT_LIFETIME_INACTIVE 0

enum CharacterType
//Could also be subtypes of NPC/PC
{
    NPC = 0,    //Non-Player Character (not human controlled, generic type)
    PC          //Player Character (human-controlled)
};

enum LocationType
{
    WILDERNESS = 0,
    CAVE = 1
};

enum TileFeatureFlag
//Tile features seen by player
//Only one feature active at one time
{
    NO_FEATURE = 0,
    VEGETATION_FEATURE,
    WALL_FEATURE
};

enum TileAttributeFlag
//Tile attributes
//Many attributes can be active at one time
{
    NO_ATTRIBUTE = 0,
    IMPASSIBLE_ATTRIBUTE = (1 << 1) //Shift left 1
};

#endif
