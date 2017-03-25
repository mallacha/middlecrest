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
    index = 0;
    characterID = 0;
    xNew = 0;
    yNew = 0;
    zNew = 0;
    //xPrevious = 0;
    //yPrevious = 0;
    //zPrevious = 0;
}

moveCharacter::~moveCharacter()
//Base class destructor
{
    index = 0;
    characterID = 0;
    xNew = 0;
    yNew = 0;
    zNew = 0;
    //xPrevious = 0;
    //yPrevious = 0;
    //zPrevious = 0;
}

moveCharacter::moveCharacter(unsigned long characterIndex, character * object, unsigned short x, unsigned short y, unsigned short z)
{
    index = characterIndex;
    characterID = object->id.getID();
    xNew = x;
    yNew = y;
    zNew = z;
    //xPrevious = object->getXCoord();
    //yPrevious = object->getYCoord();
    //zPrevious = object->getZCoord();
}

void moveCharacter::execute()
//Move character
{
    characterPool * characters = locator::getCharacters();

    if(characters->getCharacterIdByPoolId(index) == characterID) {

        //Save character's old coordinates
        //xPrevious = characters->getXCoordByPoolId(index);
        //yPrevious = characters->getYCoordByPoolId(index);
        //zPrevious = characters->getZCoordByPoolId(index);

        //Set new coordinaties for character
        characters->setXCoordByPoolId(index, xNew);
        characters->setYCoordByPoolId(index, yNew);
        characters->setZCoordByPoolId(index, zNew);
    }
}

void handleInput(character * actor, unsigned long ith)
{
    //Wait for input
    ch = getch();

    //Get services
    IEvents * events = locator::getEvents();
    IEvent * event = NULL;
    IEvent * command = NULL;
    IDisplay * display = locator::getDisplay();

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

        #ifdef ASSERTION_TEST
            //Continues the else-if and checks for NULL event object if no event was generated
            else {
                assert(NULL == event);
            }
        #endif

    if(command) {
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
