#ifndef _SERVICE_H
#define _SERVICE_H

#pragma once
#include "lib.h"
#include "logging.hpp"
#include "ascii.hpp"
#include "database.hpp"
#include "randomc.hpp"
#include "characterPool.hpp"
#include "event.hpp"
#include "game.hpp"

    #ifdef UNIT_TEST
        #include "../tests/includes/testHarness.hpp"
    #endif

class locator
//Service Locator
{
    public:
        locator() { RNGService=NULL; loggingService=NULL; databaseService=NULL; displayService=NULL;
                    characterService=NULL; eventService=NULL; gameService=NULL;

                          #ifdef UNIT_TEST
                              unitTestService=NULL;
                          #endif
                  }
        ~locator() { delete RNGService; delete characterService; delete databaseService;
                     delete eventService; delete displayService; delete loggingService;
                     delete gameService;

                          #ifdef UNIT_TEST
                              delete unitTestService;
                          #endif
                }

        //Service getters
        static void initialize() { RNGService=&nullRNGService; loggingService = new logging();
                                   loggingService->info("Registered logging service via service constructor");
                                   databaseService=&nullDatabaseService; displayService=&nullDisplayService;
                                   eventService=&nullEventService; characterService=&nullCharacterService;
                                   gameService=&nullGameService;

                                       #ifdef UNIT_TEST
                                           unitTestService=&nullUnitTestService;
                                       #endif
                                  }
        inline static ILogging * getLogging() { return loggingService; }
        inline static IDisplay * getDisplay() { return displayService; }
        inline static IDatabase * getDatabase() { return databaseService; }
        inline static IRand * getRNG() { return RNGService; }
        inline static characterPool * getCharacters() { return characterService; }
        inline static IEvents * getEvents() { return eventService; }
        inline static IGame * getGame() { return gameService; }


            #ifdef UNIT_TEST
                inline static IUnitTest * getUnitTests() { return unitTestService; }
            #endif
        //Registration functions
        static void provide(ILogging * const);
        static void provide(IDisplay * const);
        static void provide(IDatabase * const);
        static void provide(IRand * const);
        static void provideCharacterService(characterPool * const);
        static void provide(IEvents * const);
        static void provide(IGame * const);


            #ifdef UNIT_TEST
                static void provide(IUnitTest * const);
            #endif
    private:
        static ILogging * loggingService;
        static IDisplay * displayService;
        static IDatabase * databaseService;
        static IRand * RNGService;
        static characterPool * characterService;
        static IEvents * eventService;
        static IGame * gameService;


            #ifdef UNIT_TEST
                static IUnitTest * unitTestService;
            #endif
        static nullLog nullLoggingService;
        static nullDisplay nullDisplayService;
        static nullDatabase nullDatabaseService;
        static nullRNG nullRNGService;
        static nullCharacter nullCharacterService;
        static nullEventsObject nullEventService;
        static nullGame nullGameService;


            #ifdef UNIT_TEST
                static nullUnitTest nullUnitTestService;
            #endif
};

#endif
