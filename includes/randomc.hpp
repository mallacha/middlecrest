/***************************** RANDOMC.H *********************** 2001-10-24 AF *
*
* This file contains class declarations for the C++ library of uniform
* random number generators.
*
* Overview of classes:
* ====================
*
* class TRanrotBGenerator:
* Random number generator of type RANROT-B.
* Source file ranrotb.cpp
*
* class TRanrotWGenerator:
* Random number generator of type RANROT-W.
* Source file ranrotw.cpp
*
* class TRandomMotherOfAll:
* Random number generator of type Mother-of-All (Multiply with carry).
* Source file mother.cpp
*
* class TRandomMersenne:
* Random number generator of type Mersenne twister.
* Source file mersenne.cpp
*
* class TRandomMotRot:
* Combination of Mother-of-All and RANROT-W generators.
* Source file ranmoro.cpp and motrot.asm.
* Coded in assembly language for improved speed.
* Must link in RANDOMAO.LIB or RANDOMAC.LIB.
*
*
* Member functions (methods):
* ===========================
*
* All these classes have identical member functions:
*
* Constructor(long int seed):
* The seed can be any integer. Usually the time is used as seed.
* Executing a program twice with the same seed will give the same sequence of
* random numbers. A different seed will give a different sequence.
*
* double Random();
* Gives a floating point random number in the interval 0 <= x < 1.
* The resolution is 32 bits in TRanrotBGenerator, TRandomMotherOfAll and
* TRandomMersenne. 52 or 63 bits in TRanrotWGenerator. 63 bits in
* TRandomMotRot.
*
* int IRandom(int min, int max);
* Gives an integer random number in the interval min <= x <= max.
* The resolution is the same as for Random().
*
* unsigned long BRandom();
* Gives 32 random bits.
* Only available in the classes TRanrotWGenerator and TRandomMersenne.
*
*
* Example:
* ========
* The file EX-RAN.CPP contains an example of how to generate random numbers.
*
*
* Further documentation:
* ======================
* The file randomc.htm contains further documentation on these random number
* generators.
*
*******************************************************************************/

#ifndef RANDOMC_H
#define RANDOMC_H

#pragma once
#include "lib.h"
#include "logging.hpp"

#ifdef HIGH_RESOLUTION
typedef long double trfloat;        // use long double precision
#else
typedef double trfloat;             // use double precision
#endif

class IRand
//Interface for RNG
{
    public:
        virtual ~IRand();

        virtual long randomInit(long) = 0;//re-seed generator
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

/*class TRandomMersenne : public IRand {                  // encapsulate random number generator
  enum constants {
#if 1
    // define constants for MT11213A
    N = 351, M = 175, R = 19, MATRIX_A = 0xEABD75F5,
    TEMU = 11, TEMS = 7, TEMT = 15, TEML = 17, TEMB = 0x655E5280, TEMC = 0xFFD58000
#else
    // or constants for MT19937
    N = 624, M = 397, R = 31, MATRIX_A = 0x9908B0DF,
    TEMU = 11, TEMS = 7, TEMT = 15, TEML = 18, TEMB = 0x9D2C5680, TEMC = 0xEFC60000
#endif
  };
  public:
      TRandomMersenne();
      TRandomMersenne(long int value) {           // constructor
          RandomInit(value);}
      unsigned long RandomInit(long int);            // re-seed
      long IRandom(long min, long max);          // output random integer
      inline long int getSeed() { return seed; }

  private:
      double Random();                           // output random float

      unsigned long BRandom();                   // output random bits
      unsigned long mt[N];                       // state vector
      long int seed;                             // seed
      int mti;                                   // index into mt
};*/

class nullRNG : public IRand
{
    public:
        nullRNG() { logger=NULL; }
        ~nullRNG() {}
        inline long randomInit(long int value) { log(); return 0; }
        inline long intRandom(long value1, long value2) { log(); return 0; }
        inline long getSeed() { log(); return 0; }

            #ifdef UNIT_TEST
                inline unsigned long BRandom() { log(); return 0; }
            #endif

    private:
        void log();

        ILogging * logger;
};

#endif
