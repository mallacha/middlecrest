#include "../includes/service.hpp"
#include "../includes/utility.hpp"

using namespace std;

/**********************************************************
                    Character attributes
***********************************************************/

characterAttributes::characterAttributes()
{
    strength=0;
    agility=0;
    coordination=0;
    intuition=0;
    willpower=0;
    endurance=0;
    intelligence=0;
    wits=0;
    appearence=0;
    charisma=0;
}

characterAttributes::~characterAttributes()
{
    strength=0;
    agility=0;
    coordination=0;
    intuition=0;
    willpower=0;
    endurance=0;
    intelligence=0;
    wits=0;
    appearence=0;
    charisma=0;
}



/**********************************************************
                     Character location
***********************************************************/

characterLocation::characterLocation()
{
    worldX=3;
    worldY=11;
    zoneX=0;
    zoneY=0;
    x=0;
    y=0;
    z=0;
}


/*char * characterLocation::serialize()
{
    int maxSize = 75;

            #ifdef ASSERTION_TEST
            //Tests whether the (if(!serialized) below is ever hit
            //this test leads into the subsequent test that makes
            //sure buffer is wide enough
                maxSize=1000;

                if(serialized) {
                    delete [] serialized;
                }
            #endif

    //Create buffer
    if(!serialized) {
        serialized = new char[maxSize];
    }

            #ifdef ASSERTION_TEST
                snprintf(serialized, maxSize, "{%d:%d:%d:%d:%d,%d,%d}", worldX, worldY,zoneX,zoneY,x,y,z);

                //Making sure buffer is large enough. If not, increase.
                assert(strlen(serialized) < maxSize);

                //Reset to original size and carry on
                delete [] serialized;
                serialized = new char[maxSize];
            #endif

    //Print
    snprintf(serialized, maxSize, "{%d:%d:%d:%d:%d:%d:%d}", worldX, worldY,zoneX,zoneY,x,y,z);

    return serialized;
}*/

characterLocation::~characterLocation()
{
    worldX=0;
    worldY=0;
    zoneX=0;
    zoneY=0;
    x=0;
    y=0;
    z=0;
}



/**********************************************************
                          Character
***********************************************************/

character::character()
{
    type=NPC;
    sigil='p'; //default. Typically not used
    save_id=0;
    object = new gameObject();
}

character::~character()
{
    type=NPC;
    sigil=' ';
    save_id=0;
    if(object) {
        delete object;
    }
}

void character::setType(CharacterType cType)
//Set specific values
{
   IGame * game = locator::getGame();
   type = cType;
   duration * lifetimeObj = lifetimeObject.getLifetimeObject();

   switch(type) {
       case PC:
           lifetimeObj->set(OBJECT_LIFETIME_PERMANENT);
           sigil='P';
           save_id = game->getPlayerInstance();
         break;
       default:
           lifetimeObj->set(OBJECT_LIFETIME_LONG);
         break;
   }
}

void character::setPermanent()
//Set permanant object
{
//    initialize();
//    objectLifetime.setLifetime(OBJECT_LIFETIME_PERMANENT);
}

void character::setTemporary()
//Set temporary object
{
//    initialize();
//    objectLifetime.setLifetime(OBJECT_LIFETIME_LONG);
}

void character::setXCoord(unsigned long value)
{
    value = (int)validateUnsignedShort(value);

    location.x = (unsigned short)value;
}

void character::setYCoord(unsigned long value)
{
    value = (int)validateUnsignedShort(value);

    location.y = (unsigned short)value;
}

void character::setZCoord(unsigned long value)
{
    value = (int)validateUnsignedShort(value);

    location.z = (unsigned short)value;
}

void generateStartingLocation()
{

}

void character::save()
{
    if(lifetimeObject.isActive()) { //TODO, this needs to change to be more amenative to adding and removing characters from db but also the lifetime state (which, I'm not sure of right now)
        IDatabase * database = locator::getDatabase();
        sqlite3_stmt* res;

        if(save_id) {
        //Save character data to database

        } else {
            //TODO: mid needs to change. Refernce the variable in cahracter
            database->saveQuery(res, "INSERT INTO characters (mid) VALUES (1);", 75);
            sqlite3_finalize(res);

            //Grab last character saved (above) to get save_id
            database->saveQuery(res, "SELECT id FROM characters ORDER BY id DESC LIMIT 1;", 55);
            save_id = (unsigned int)sqlite3_column_int(res, 0);
            sqlite3_finalize(res);

            if(type == PC) {
            //If character is PC, update character to be a player-character
                database->saveQuery(res, "UPDATE characters SET player_character_bool=1 WHERE id=%d;", 70, save_id);
                sqlite3_finalize(res);
            }

            IGame * game = locator::getGame();
            if(!game->getPlayerInstance()) {
            //If no player character set for this game, assign character to be
            //player-character for this game
                game->setPlayerInstance(save_id);
            }
        }
    }
}
