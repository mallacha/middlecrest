#ifndef RANDOMC_H
#define RANDOMC_H

#pragma once
#include "lib.h"
#include "logging.hpp"

class IRand
//Interface for RNG
{
    public:
        virtual ~IRand();

        virtual long randomInit(long) = 0; //re-seed generator
        virtual long intRandom(long, long) = 0; //random integer
        virtual long int getSeed() = 0;
};

class simpleRandom : public IRand
//Simple RNG using rand()
{
    public:
        simpleRandom();
        ~simpleRandom();

        inline long randomInit(long int);
        inline long intRandom(long, long);
        inline long int getSeed() { return seed; }

    private:
        inline unsigned int random();

        unsigned long next;
        unsigned long seed;
};

class nullRNG : public IRand
{
    public:
        nullRNG() { logger=NULL; }
        ~nullRNG() {}
        inline long randomInit(long int value) { log(); return 0; }
        inline long intRandom(long value1, long value2) { log(); return 0; }
        inline long getSeed() { log(); return 0; }

    private:
        void log();

        ILogging * logger;
};

#endif
