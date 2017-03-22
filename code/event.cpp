#include "../includes/event.hpp"
#include "../includes/service.hpp"

using namespace std;

eventObject::eventObject()
{
    priority = SHRT_MIN;
    event = NULL;
}

eventObject::eventObject(IEvent * object, unsigned int value)
//Construct object with a specific event and priority
{
    if(object) {
        event = object;

        if(value < SHRT_MIN) {
            value = SHRT_MIN;
        } else if(value > SHRT_MAX) {
            value = SHRT_MAX;
        }
        priority = value;
    }
}

eventObject::~eventObject()
{
    priority = 0;
    if(NULL != event) {
        delete event;
    }
}


events::events()
{
    //TODO: tune to appropriate size once there is relevant data
    objects.reserve(10);
}

void events::add(IEvent * object)
//Add object to events queue
{
        #ifdef ASSERTION_TEST
            assert(object != NULL);
        #endif
    if(object) {
        objects.push_back(object);
    }
}

void events::execute()
//Executes all events in order of priority
//TODO: This method will need objects to be sorted. Currently they only have one priority value.
{
    IEvent * object=NULL;

    while(!objects.empty()) {
        object = objects.back();
        object->execute();
        objects.pop_back();
    }

        #ifdef ASSERTION_TEST
            assert(objects.empty());
        #endif
}

void nullEventsObject::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullEventsObject object.");
}
