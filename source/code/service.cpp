#include "../includes/service.hpp"

using namespace std;

//MCLocation * locator::locationService;
IZoneCache * locator::zoneService;
ILogging * locator::loggingService;
IDisplay * locator::displayService;
IDatabase * locator::databaseService;
IRand * locator::RNGService;
characterPool * locator::characterService;
IEvents * locator::eventService;
IGame * locator::gameService;
//nullLocation locator::nullLocationService;
nullZoneCache locator::nullZoneService;
nullLog locator::nullLoggingService;
nullDisplay locator::nullDisplayService;
nullDatabase locator::nullDatabaseService;
nullRNG locator::nullRNGService;
nullCharacter locator::nullCharacterService;
nullEventsObject locator::nullEventService;
nullGame locator::nullGameService;

void locator::provide(IZoneCache * const service)
//Register location service
{
    if(service == NULL){
        // Revert to null service
        zoneService = &nullZoneService;
    } else {
        zoneService = service;
        loggingService->info("Registered location service");
    }
}

void locator::provide(ILogging * const service)
//Register logging service
{
    if(service == NULL){
        // Revert to null service
        loggingService = &nullLoggingService;
    } else {
        loggingService = service;
        loggingService->info("Registered logging service via provider");
    }
}

void locator::provide(IDisplay * const service)
//Register display service
{
    if(service == NULL){
        // Revert to null service
        displayService = &nullDisplayService;
    } else {
        displayService = service;
        loggingService->info("Registered display service");
    }
}

void locator::provide(IDatabase * const service)
//Register database service
{
    if(service == NULL){
        // Revert to null service
        databaseService = &nullDatabaseService;
    } else {
        databaseService = service;
        loggingService->info("Registered database service");
    }
}

void locator::provide(IRand * const service)
//Register RNG service
{
    if(service == NULL){
        // Revert to null service
        RNGService = &nullRNGService;
    } else {
        RNGService = service;
        loggingService->info("Registered RNG service");
    }
}

void locator::provide(characterPool * const service)
//Register character pool
{
    if(service == NULL){
        // Revert to null service
        characterService = &nullCharacterService;
    } else {
        characterService = service;
        loggingService->info("Registered character pool service");
    }
}

void locator::provide(IEvents * const service)
//Register object that handles events
{
    if(service == NULL) {
        eventService = &nullEventService;
    } else {
        eventService = service;
        loggingService->info("Registered event service");
    }
}

void locator::provide(IGame * const service)
//Register world
{
    if(service == NULL) {
        gameService = &nullGameService;
    } else {
        gameService = service;
        loggingService->info("Registered game service");
    }
}
