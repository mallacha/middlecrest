#ifndef _DATABASE_H
#define _DATABASE_H

#pragma once
#include "../../core/includes/lib.h"
#include "../../core/includes/logging.hpp"
#include "sqlite3.h"

class IDatabase
//Mock interface since this is not used as part of the core library
{
    public:
        virtual int dataQuery(sqlite3_stmt *&, const char *, short, ...) = 0;
        virtual int saveQuery(sqlite3_stmt *&, const char *, short, ...) = 0;
        virtual int fileQuery(sqlite3_stmt *&, const char *, const char *, short, ...) = 0;
};

class sqliteDatabase : public IDatabase
//Database. Allows easier debugging and standardization of database behaviors.
{
    public:
        sqliteDatabase();
        ~sqliteDatabase();

        int dataQuery(sqlite3_stmt *&, const char *, short, ...); //Repository for generic game data
        int saveQuery(sqlite3_stmt *&, const char *, short, ...); //Repository for saved game data
        int fileQuery(sqlite3_stmt *&, const char *, const char *, short, ...); //Repository for game languages
        //localizationQuery                                                         //Repository for localization

    private:
        int retrieve(sqlite3_stmt *&, const char *&, const char *&, short &, va_list &);

        unsigned short queryResult; //Query status
        ILogging * logger; //Logging object
        char * query; //Query -- character buffer
};

class nullDatabase : public IDatabase
//Null database
{
    public:
        nullDatabase() { logger=NULL; }
        ~nullDatabase() { logger=NULL; }

        inline int dataQuery(sqlite3_stmt *&, const char *, short, ...) { log(); }
        inline int saveQuery(sqlite3_stmt *&, const char *, short, ...) { log(); }
        inline int fileQuery(sqlite3_stmt *&, const char *, const char *, short, ...) { log(); }

    private:
        void log();
        ILogging * logger;
};

#endif
