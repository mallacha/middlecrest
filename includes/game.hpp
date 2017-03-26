#ifndef _GAME_H
#define _GAME_H

#pragma once

#include "../includes/lib.h"
#include "../includes/logging.hpp"
#include "../includes/utility.hpp"

class IGame
{
    public:
        IGame();
        virtual ~IGame();

        //Getters
        inline unsigned int getPlayerInstance() { return (long)player_instance; }
        int gameRunning() { return running; } //true

        //Setters
        virtual void save() {}
        virtual void initialize();
        inline virtual void setPlayerInstance(unsigned int value) { player_instance = validateUnsignedInt(value); }
        void setSignalQuit() { running=0; }

    private:
        inline void createWorldCellTable();
        inline void createObjectTable();

        unsigned int player_instance; //Database id, not game object id. So we know where to store it
        unsigned short running; //playing game state
};

class middlecrest : public IGame
//Game object that organizes state and operations for Middlecrest
{
    public:
        middlecrest();
        ~middlecrest();

        //Getters

        //Setters
        void save();
        void initialize();

    private:
};

class nullGame : public IGame
{
    public:
        nullGame();
        ~nullGame();

        //Setters
        inline void initialize() { log(); }

    private:
        void log();
        ILogging * logger;
};

#endif
