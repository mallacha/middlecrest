#ifndef _LOGGING_CORE_H
#define _LOGGING_CORE_H

#pragma once
#include "lib.h"

using namespace std;

class ILogging
//Interface for logging
{
    public:
        virtual void info(const char * format, ...) = 0; //Business process has finished.
        virtual void trace(const char * format, ...) = 0; //Detailed information intended for development.
        virtual void debug(const char * format, ...) = 0; //Debug information
        virtual void attn(const char * format, ...) = 0; //Warning Level 1 (Attention)
        virtual void warn(const char * format, ...) = 0; //Warning Level 2 (Warning)
        virtual void error(const char * format, ...) = 0; //Error
};

class logging : public ILogging
//Log object
{
    public:
        logging();
        ~logging();

        void info(const char * format, ...); //Business process has finished.
        void trace(const char * format, ...); //Detailed information intended for development.
        void debug(const char * format, ...); //Debug information
        void attn(const char * format, ...); //Warning Level 1 (Attention)
        void warn(const char * format, ...); //Warning Level 2 (Warning)
        void error(const char * format, ...); //Error
        //Tips on logging: http://www.javacodegeeks.com/2011/01/10-tips-proper-application-logging.html

    protected:
        char * textBuffer;
};

class nullLog : public ILogging
//Null log
{
    public:
        nullLog() {}
        ~nullLog() {}

        void info(const char * format, ...) {} //Business process has finished.
        void trace(const char * format, ...) {} //Detailed information intended for development.
        void debug(const char * format, ...) {} //Debug information
        void attn(const char * format, ...) {} //Warning Level 1 (Attention)
        void warn(const char * format, ...) {} //Warning Level 2 (Warning)
        void error(const char * format, ...){} //Error
};

#endif
