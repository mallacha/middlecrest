#ifndef _SERVICE_H
#define _SERVICE_H

#pragma once
#include "../../core/includes/lib.h"
#include "../../core/includes/logging.hpp"
#include "location.hpp"
#include "display.hpp"
#include "database.hpp"
#include "randomc.hpp"
#include "pool.hpp"
#include "event.hpp"
#include "game.hpp"

class locator
//Service Locator
{
    public:
        locator() { zoneService=NULL; loggingService=NULL; displayService=NULL; databaseService=NULL;
                    RNGService=NULL; characterService=NULL; eventService=NULL; gameService=NULL; }
        ~locator() {}

        //Service getters
        static void initialize() { zoneService=&nullZoneService; loggingService = new logging(); loggingService->info("Registered logging service via service constructor");
                                   displayService=&nullDisplayService; databaseService=&nullDatabaseService;
                                   RNGService=&nullRNGService; characterService=&nullCharacterService; eventService=&nullEventService;
                                   gameService=&nullGameService; }
        inline static IZoneCache * getLocation() { return zoneService; }
        inline static ILogging * getLogging() { return loggingService; }
        inline static IDisplay * getDisplay() { return displayService; }
        inline static IDatabase * getDatabase() { return databaseService; }
        inline static IRand * getRNG() { return RNGService; }
        inline static characterPool * getCharacters() { return characterService; }
        inline static IEvents * getEvents() { return eventService; }
        inline static IGame * getWorld() { return gameService; }

        //Registration functions
        static void provide(IZoneCache * const);
        static void provide(ILogging * const);
        static void provide(IDisplay * const);
        static void provide(IDatabase * const);
        static void provide(IRand * const);
        static void provide(characterPool * const);
        static void provide(IEvents * const);
        static void provide(IGame * const);

    private:
        static IZoneCache * zoneService;
        static ILogging * loggingService;
        static IDisplay * displayService;
        static IDatabase * databaseService;
        static IRand * RNGService;
        static characterPool * characterService;
        static IEvents * eventService;
        static IGame * gameService;

        static nullZoneCache nullZoneService;
        static nullLog nullLoggingService;
        static nullDisplay nullDisplayService;
        static nullDatabase nullDatabaseService;
        static nullRNG nullRNGService;
        static nullCharacter nullCharacterService;
        static nullEventsObject nullEventService;
        static nullGame nullGameService;
};

#endif
