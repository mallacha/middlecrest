#ifndef _DATABASE_H
#define _DATABASE_H

#pragma once
#include "lib.h"
#include "logging.hpp"
#include "sqlite3.h"

class IDatabase
//Mock interface since this is not used as part of the core library
{
    public:
        virtual ~IDatabase() = 0;

        virtual int dataQuery(sqlite3_stmt *&, const char *, short, ...) = 0;
        virtual int saveQuery(sqlite3_stmt *&, const char *, short, ...) = 0;
        virtual int fileQuery(sqlite3_stmt *&, const char *, const char *, short, ...) = 0;
};

//Helpers
int saveDataTableCreation(const char *, int, const char *, int);

class sqliteDatabase : public IDatabase
//Database. Allows easier debugging and standardization of database behaviors.
{
    public:
        sqliteDatabase();
        ~sqliteDatabase();

        int dataQuery(sqlite3_stmt *&, const char *, short, ...); //Repository for generic game data
        int saveQuery(sqlite3_stmt *&, const char *, short, ...); //Repository for saved game data
        int fileQuery(sqlite3_stmt *&, const char *, const char *, short, ...); //Repository for game languages
        //localizationQuery                                                           //Repository for localization

    private:
        int retrieve(sqlite3_stmt *&, const char *&, const char *&, short &, va_list &);
        void createFile(int &, const char *); //Create any files on filesystem

        unsigned short queryResult; //Query status
        ILogging * logger; //Logging object
        sqlite3* dbase; //database
        char * query; //Query -- character buffer
        char * logBuffer; //Buffer for outputting to log
};

class nullDatabase : public IDatabase
//Null database
{
    public:
        nullDatabase() { logger=NULL; }
        ~nullDatabase() { logger=NULL; }

        inline int dataQuery(sqlite3_stmt *&, const char *, short, ...) { log(); return 0; }
        inline int saveQuery(sqlite3_stmt *&, const char *, short, ...) { log(); return 0; }
        inline int fileQuery(sqlite3_stmt *&, const char *, const char *, short, ...) { log(); return 0; }

    private:
        void log();
        ILogging * logger;
};

#endif
