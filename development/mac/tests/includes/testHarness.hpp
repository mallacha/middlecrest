#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#pragma once
#include "randomcTest.hpp"
#include "poolTest.hpp"
#include "characterPoolTest.hpp"
#include "locationTest.hpp"
#include "displayTest.hpp"

class IUnitTest
{
    public:
        virtual void run() = 0;
        virtual void add(const char *) = 0;
        virtual void displayResult(short) = 0;
        virtual void incCoverageTotal(short) = 0;
        virtual void incCoverageCurrent(short) = 0;
};

class testHarness : public IUnitTest
{
    public:
        testHarness();
        ~testHarness();

        void run();
        void add(const char *);
        void displayResult(short);
        void incCoverageTotal(short value) { codeCoverageTotal += value; }
        void incCoverageCurrent(short value) { codeCoverageCurrent += value; }

    private:
        void RNGTests();
        void basicPoolTests();
        void mapTests();
        void displayTests();
        void begin();
        void stop();
        void report();

        clock_t start;
        clock_t end;
        //Rough guesstimate at coverage. Created manually by expressing two metrics: 1) number of lines covered, and 2) code cases covered.
        //Each line gets a point, each actual test receives a point, and each case gets a point. codeCoverageTotal is the sum of these
        //points and codeCoverageCurrent is the measure of how many of those are actually covered.
        unsigned int codeCoverageTotal;
        unsigned int codeCoverageCurrent;
        float filesCovered;

        unsigned int testCount;
        unsigned short succeses;
        unsigned short failures;
        unsigned short skips;
};

class nullUnitTest : public IUnitTest
//Null location
{
    public:
        nullUnitTest() { logger=NULL; }
        ~nullUnitTest() { logger=NULL; }

        //Interface functions
        void run() { log(); }
        void add(const char * value) { log(); }
        void displayResult(short value) { log(); }
        void incCoverageTotal(short value) { log(); }
        void incCoverageCurrent(short value) { log(); }

    private:
        void log();

        ILogging * logger;
};

#endif
