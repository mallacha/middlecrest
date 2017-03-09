#ifndef _EVENT_H
#define _EVENT_H

#pragma once
#include "../../core/includes/event.hpp"
#include "../../core/includes/logging.hpp"
#include "configuration.hpp"

class eventObject : public IObject
//Information to perform an event and set its priority
{
    public:
        eventObject();
        eventObject(IObject *, unsigned int);
        ~eventObject();

        inline void execute() { event->execute(); }

    private:
        IObject * event;
        short priority;
};

class events : public IEvents
//Collects and executes events
{
    public:
        events();
        ~events() {}

        void add(IObject *);
        void execute();

    private:
        vector<IObject *> objects;
};

class nullEventsObject : public IEvents
//Null events object
{
    public:
        nullEventsObject() { logger=NULL; }
        ~nullEventsObject() { logger=NULL; }

        //Interface functions
        void execute() { log(); }
        void add(IObject * event) { log(); }

    private:
        void log();

        ILogging * logger;
};

#endif
