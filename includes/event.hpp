#ifndef _EVENT_H
#define _EVENT_H

#pragma once
#include "lib.h"
#include "logging.hpp"
#include <vector>

using namespace std;

/**********************************************************
                       Event Object
***********************************************************/

class IEvent
//Object interface
{
    public:
        IEvent() {}
        ~IEvent() {}

        virtual void execute() {} //Used by: events
};

class eventObject : public IEvent
//Information to perform an event and set its priority
{
    public:
        eventObject();
        eventObject(IEvent *, unsigned int);
        ~eventObject();

        inline void execute() { event->execute(); }

    private:
        IEvent * event;
        short priority;
};

/**********************************************************
                        Event Stack
***********************************************************/

class IEvents
//Game events interface (movement, messages, etc.)
{
    public:
        IEvents() {}
        ~IEvents() {}

        virtual void execute() = 0;
        virtual void add(IEvent *) = 0;
};

class events : public IEvents
//Collects and executes events
{
    public:
        events();
        ~events() {}

        void add(IEvent *);
        void execute();

    private:
        vector<IEvent *> objects;
};

class nullEventsObject : public IEvents
//Null events object
{
    public:
        nullEventsObject() { logger=NULL; }
        ~nullEventsObject() { logger=NULL; }

        //Interface functions
        void execute() { log(); }
        void add(IEvent * event) { log(); }

    private:
        void log();

        ILogging * logger;
};

#endif
