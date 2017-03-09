#include "../includes/character.hpp"
#include "../includes/service.hpp"

using namespace std;

character::character()
{
    type=NPC;
    x=0;
    y=0;
    z=0;
    zoneID=0;
    sigil='p';
}

character::~character()
{
    type=NPC;
    x=0;
    y=0;
    z=0;
    zoneID=0;
    sigil=' ';
}

void character::setType(CharacterType cType)
//Set specific values
{
   type = cType;
   switch(type) {
       case PC:
           objectLifetime.setLifetime(OBJECT_LIFETIME_PERMANENT);
           sigil='P';
         break;
       default:
           objectLifetime.setLifetime(OBJECT_LIFETIME_LONG);
         break;
   }
}

void character::setPermanent()
//Set permanant object
{
    initialize();
    objectLifetime.setLifetime(OBJECT_LIFETIME_PERMANENT);
}

void character::setTemporary()
//Set temporary object
{
    initialize();
    objectLifetime.setLifetime(OBJECT_LIFETIME_LONG);
}

void character::moveCharacter(unsigned short xNew, unsigned short yNew, unsigned short zNew)
{
    x = xNew;
    y = yNew;
    z = zNew;
}
