#ifndef _COMMAND_H
#define _COMMAND_H

#pragma once
#include "character.hpp"
#include "event.hpp"

class moveCharacter : public IEvent
//Move character
{
    public:
        moveCharacter();
        moveCharacter(unsigned long, character *, unsigned short, unsigned short, unsigned short);
        ~moveCharacter();

        void execute();
        void undo();

    private:
        unsigned long characterIndex; //index in character array
        unsigned long characterID; //id of character
        unsigned short xNew;
        unsigned short yNew;
        unsigned short zNew;
        unsigned short xPrevious;
        unsigned short yPrevious;
        unsigned short zPrevious;
};

class commandPickUp : public IEvent
{
    public:
        commandPickUp();
        commandPickUp(unsigned long, unsigned long, unsigned short, unsigned short, unsigned short);
        ~commandPickUp();

    private:
        unsigned long characterIndex; //index in character array
        unsigned long characterID; //id of character
        unsigned long itemID;
        unsigned short squareX;
        unsigned short squareY;
        unsigned short squareZ;
};

void handleInput(character *, unsigned long);
void switchKeyBindings(const unsigned short &);

#endif
