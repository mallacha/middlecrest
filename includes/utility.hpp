#ifndef _UTILITY_H
#define _UTILITY_H

#pragma once

#include "../includes/lib.h"

int validateUnsignedShort(long);
unsigned int validateUnsignedInt(long);
//void sleep(float); //sleeps specified number of seconds


        #ifdef UNIT_TEST
            void timerSet(int);
            int timerFinished();
        #endif

#endif
