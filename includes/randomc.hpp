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

        virtual long randomInit(unsigned int) = 0; //re-seed generator
        virtual unsigned int intRandom(long, long) = 0; //random integer
        virtual long int getSeed() = 0;
};

class simpleRandom : public IRand
//Simple RNG using rand()
{
    public:
        simpleRandom();
        ~simpleRandom();

        inline long randomInit(unsigned int);
        inline unsigned int intRandom(long, long);
        inline long int getSeed() { return seed; }

    private:
        inline unsigned int random();

        unsigned long int next;
        unsigned long seed;
};

class nullRNG : public IRand
{
    public:
        nullRNG() { logger=NULL; }
        ~nullRNG() {}
        inline long randomInit(unsigned int value) { log(); return 0; }
        inline unsigned int intRandom(long value1, long value2) { log(); return 0; }
        inline long getSeed() { log(); return 0; }

    private:
        void log();

        ILogging * logger;
};

#endif
