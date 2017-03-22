#include "../includes/command.hpp"
#include "../includes/service.hpp"

using namespace std;

static unsigned short keybinding = 0;
static char ch = ' ';

//Default keybinding
static char MOVEUP = (char)KEY_UP;
static char MOVEDOWN = (char)KEY_DOWN;
static char MOVELEFT = (char)KEY_LEFT;
static char MOVERIGHT = (char)KEY_RIGHT;
static char SEARCH = 's';
static char SWITCHKEYBINDING = 'b';

moveCharacter::moveCharacter()
//Base class constructor
{
    characterIndex = 0;
    characterID = 0;
    xNew = 0;
    yNew = 0;
    zNew = 0;
    xPrevious = 0;
    yPrevious = 0;
    zPrevious = 0;
}

moveCharacter::~moveCharacter()
//Base class destructor
{
    characterIndex = 0;
    characterID = 0;
    xNew = 0;
    yNew = 0;
    zNew = 0;
    xPrevious = 0;
    yPrevious = 0;
    zPrevious = 0;
}

moveCharacter::moveCharacter(unsigned long index, character * object, unsigned short x, unsigned short y, unsigned short z)
{
    characterIndex = index;
    characterID = object->id.getID();
    xNew = x;
    yNew = y;
    zNew = z;
    xPrevious = object->getXCoord();
    yPrevious = object->getYCoord();
    zPrevious = object->getZCoord();
}

void moveCharacter::execute()
//Move character
{
/*    IZoneCache * location = locator::getLocation();
    zone * zoneArea = location->getZone();
    tile *** dungeon = zoneArea->getGrid();*/

    characterPool * characters = locator::getCharacters();
    character * characterObject = characters->getPool();
    character * obj = NULL;

    //if(!(dungeon[zNew][xNew][yNew]).isAttribute(IMPASSIBLE_ATTRIBUTE)) {
    //If tile is impassible

        if(characterObject[characterIndex].id.getID() == characterID) { //TODO: Combine with condition above
/*cout << characterID << endl;
cout << xNew << endl;
cout << yNew << endl;
cout << zNew << endl;
system("Pause");*/
            obj = &characterObject[characterIndex];

            //Save character's old coordinates
            xPrevious = obj->getXCoord();
            yPrevious = obj->getYCoord();
            zPrevious = obj->getZCoord();

            //Set new coordinaties for character
            obj->setXCoord(xNew);
            obj->setYCoord(yNew);
            obj->setZCoord(zNew);

            obj = NULL;
        }
    //}
}

void moveCharacter::undo()
{
/*    characterPool * characters = locator::getCharacters();
    character * characterObject = characters->getPool();

    if(characterObject[characterIndex].getID() == characterID) {
        characterObject[characterIndex].moveCharacter(xPrevious, yPrevious, zPrevious);
    }*/
}

void handleInput(character * actor, unsigned long ith)
{
    //Wait for input
    ch = getch();

    //Get event service
    IEvents * events = locator::getEvents();
    IEvent * event = NULL;
    IEvent * command = NULL;

    characterPool * characters = locator::getCharacters();
    character * characterObject = characters->getCharacterByPoolId(ith);

    IDisplay * display = locator::getDisplay();

//TODO: make sure move doesn't generate an event when blocked
    if(ch == MOVEUP && !display->isImpassible(actor->getZCoord(), actor->getXCoord(), actor->getYCoord()-1)) {
        event = new moveCharacter(ith, actor, actor->getXCoord(), actor->getYCoord()-1, actor->getZCoord());
        command = new eventObject(event, 1);
    } else if(ch == MOVEDOWN && !display->isImpassible(actor->getZCoord(), actor->getXCoord(), actor->getYCoord()+1)) {
        event = new moveCharacter(ith, actor, actor->getXCoord(), actor->getYCoord()+1, actor->getZCoord());
        command = new eventObject(event, 1);
    } else if(ch == MOVELEFT && !display->isImpassible(actor->getZCoord(), actor->getXCoord()-1, actor->getYCoord())) {
        event = new moveCharacter(ith, actor, actor->getXCoord()-1, actor->getYCoord(), actor->getZCoord());
        command = new eventObject(event, 1);
    } else if(ch == MOVERIGHT && !display->isImpassible(actor->getZCoord(), actor->getXCoord()+1, actor->getYCoord())) {
        event = new moveCharacter(ith, actor, actor->getXCoord()+1, actor->getYCoord(), actor->getZCoord());
        command = new eventObject(event, 1);
    } else if(ch == SEARCH) {

    }

    if(NULL != command) {
        events->add(command);
    }

    if(ch == SWITCHKEYBINDING) {
        ++keybinding;
        keybinding%=8;
        switchKeyBindings(keybinding);
    }
}

void switchKeyBindings(const unsigned short & binding)
{
    switch(binding){
        case 0: //Arrow keys
            MOVEUP = (char)KEY_UP;
            MOVEDOWN = (char)KEY_DOWN;
            MOVELEFT = (char)KEY_LEFT;
            MOVERIGHT = (char)KEY_RIGHT;
            SEARCH = 's';
            //msg_window(displayText(155));
          break;
        case 1: //FPS
            MOVEUP = 'w';
            MOVEDOWN = 's';
            MOVELEFT = 'a';
            MOVERIGHT = 'd';
            SEARCH = 'f';
            //msg_window("FPS");
          break;
        case 2: //Dvorak
            MOVEUP = ',';
            MOVEDOWN = 'o';
            MOVELEFT = 'a';
            MOVERIGHT = 'e';
            SEARCH = 's';
            //msg_window("Dvorak");
          break;
        case 3: //Colemak
            MOVEUP = 'w';
            MOVEDOWN = 'r';
            MOVELEFT = 'a';
            MOVERIGHT = 's';
            SEARCH = 'f';
            //msg_window("Colemak");
          break;
        case 4: //QWERTZ
            MOVEUP = 'w';
            MOVEDOWN = 's';
            MOVELEFT = 'a';
            MOVERIGHT = 'd';
            SEARCH = 'f';
            //msg_window("QWERTZ");
          break;
/*        case 5: //AZERTY
            MOVEUP = 'z';
            MOVEDOWN = 's';
            MOVELEFT = 'q';
            MOVERIGHT = 'd';
            SEARCH = 'r';
            //msg_window("AZERTY");
            //French
          break;*/
/*        case 6: //Bépo
            MOVEUP = 'é';
            MOVEDOWN = 'u';
            MOVELEFT = 'a';
            MOVERIGHT = 'i';
            SEARCH = 's';
            //msg_window(displayText1(2));
            //Optimized French, Esperanto
          break;
        case 7: //Turkish F
            MOVEUP = 'g';
            MOVEDOWN = 'i';
            MOVELEFT = 'u';
            MOVERIGHT = 'e';
            SEARCH = 'a';
            //msg_window(displayText(156));
            //Optimized Turkish
          break;*/
        default:
            #ifdef DEBUG
                    logging * logger = new logging();
                    locator::provide(logger);

                    logger->error("Switch has no value for keybinding %d in input handler.", keybinding);
            #endif
        break;
    }
}
