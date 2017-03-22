/*****************************************
 Includes for libraries that are available
 to all code
*****************************************/

#ifndef _LIB_H
#define _LIB_H

#pragma once

//#defines for software stages of testing. Heirarchical ranking.
//These are defined during compile-time
/* #define BETA            1 //Code stays in until done with beta testing
 * #define DEBUG           1 //During development
 * #define ASSERTION_TEST  1 //During development. Usually turned off same time as DEBUG
 * #define UNIT_TEST       1 //Used while unit testing
 */

    /*************************
     Standard library
    *************************/

    #include <stdio.h>
    #include <stdlib.h>
    //#include <stdarg.h>
    #include <time.h>
    #include <limits.h>
    #include <curses.h>

    #ifdef ASSERTION_TEST
        #include <assert.h>
    #endif
/*    #include <vector>
    //#include <string>*/
    /*#ifdef UNIT_TEST
        #include <iostream>
    #endif*/

    /*************************
     C++ Boost library
    *************************/

    //#include <boost/foreach.hpp>
    //#include <boost/lexical_cast.hpp>

#endif
