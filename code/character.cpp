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
    mapId=0;
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
    mapId=0;
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
    database_id=0;
    object = new gameObject();
}

character::~character()
{
    type=NPC;
    sigil=' ';
    database_id=0;
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
           database_id = game->getPlayerInstance();
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

void character::save()
{
    if(lifetimeObject.isActive()) { //TODO, this needs to change to be more amenative to adding and removing characters from db but also the lifetime state (which, I'm not sure of right now)
        IDatabase * database = locator::getDatabase();
        sqlite3_stmt* res;

        if(database_id) {
        //Save character data to database
            if(type == PC) {
                database->saveQuery(res, "UPDATE objects SET subtype='player', type='character', coord_x=%d, coord_y=%d, mid=%d WHERE oid=%d;", 150, location.x, location.y, location.mapId, database_id);
                sqlite3_finalize(res);
            }
        } else {
            database->saveQuery(res, "INSERT INTO objects (mid) VALUES (%d);", 70, location.mapId);
            sqlite3_finalize(res);

            //Grab last character saved (above) to get save_id
            database->saveQuery(res, "SELECT oid FROM objects ORDER BY oid DESC LIMIT 1;", 55);
            database_id = (unsigned int)sqlite3_column_int(res, 0);
            sqlite3_finalize(res);

            //If no player character set for this game, assign character to be
            //player-character for this game
            IGame * game = locator::getGame();
            if(type == PC && !game->getPlayerInstance()) {
                game->setPlayerInstance(database_id);
            }
        }
    }
}
