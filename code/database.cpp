#include "../includes/database.hpp"
#include "../includes/configuration.hpp"
#include "../includes/service.hpp"
#include <fstream>

using namespace std;

#ifdef UNIT_TEST
        #ifdef DEBUG
            static const char * data_db = "../gameDataDev.db";
            static const char * save_db = "save/saveDataDev.db";
        #else
            static const char * data_db = "../gameData.db";
            static const char * save_db = "save/saveData.db";
        #endif
#else
        #ifdef DEBUG
            static const char * data_db = "gameDataDev.db";
            static const char * save_db = "save/saveDataDev.db";
        #else
            static const char * data_db = "gameData.db";
            static const char * save_db = "save/saveData.db";
        #endif
#endif

/**********************************************************
                     Helper Functions
***********************************************************/

int saveDataTableCreation(const char * select_query, int buffer1, const char * query, int buffer2)
{
    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;
    static int queryResult; //assigned next line

    queryResult = database->saveQuery(res, select_query, buffer1);

    //Create table, if needed
    if(sqlite3_column_type(res, 0) == 5) {
    //If result row is null (i.e. no table, column type is 5)
        sqlite3_finalize(res);
        queryResult = database->saveQuery(res, query, buffer2);
    }
                #ifdef DEBUG
                else {
                //Check to see if debug mode is on. If so, log.
                    ILogging * logger = locator::getLogging();
                    logger->trace("Table/data already created");
                }
                #endif

    sqlite3_finalize(res);

    return queryResult;
}



/**********************************************************
                         Database
/*********************************************************/

sqliteDatabase::sqliteDatabase()
{
    queryResult = SQLITE_OK; //Initialize to OK
    logger = locator::getLogging();
    query = NULL;
    dbase = NULL;
    logBuffer = new char[200];

    sprintf(logBuffer, "Using databases %s and %s", data_db, save_db);
    logger->info(logBuffer);

    sqlite3_initialize();
}

sqliteDatabase::~sqliteDatabase()
{
    queryResult = 0;
    logger = NULL;
    if(query) {
        delete [] query;
    }
    query = NULL;
    dbase = NULL;
    if(logBuffer) {
        delete [] logBuffer;
    }
    logBuffer=NULL;

    sqlite3_shutdown();
}

int sqliteDatabase::saveQuery(sqlite3_stmt *& res, const char * format, short bufferSize, ...)
{
    //Initialization
    va_list args;
    va_start (args, bufferSize);

    return retrieve(res, format, save_db, bufferSize, args);
}

int sqliteDatabase::dataQuery(sqlite3_stmt *& res, const char * format, short bufferSize, ...)
{
    //Initialization
    va_list args;
    va_start (args, bufferSize);

    return retrieve(res, format, data_db, bufferSize, args);
}

int sqliteDatabase::fileQuery(sqlite3_stmt *& res, const char * db, const char * format, short bufferSize, ...)
//Repository for game languages
{
    //Initialization
    va_list args;
    va_start (args, bufferSize);

    return retrieve(res, format, db, bufferSize, args);
}

int sqliteDatabase::retrieve(sqlite3_stmt *& res, const char *& format, const char *& db, short & bufferSize, va_list & args)
//Queries database with parameterized queries.
{
    if(query == NULL) {
        query = new char[bufferSize]+1;
    } else {
        #ifdef DEBUG
        //Log error. Query buffer must not be null
            logger->error("Query buffer != NULL");
        #else
        //Log warning. If this happens during production, then log at highest warning level and
        //continue.
            logger->warn("Query buffer != NULL. May result in a malformatted query. Using existing query buffer.");
        #endif
    }

    //Format query
    vsprintf(query, format, args);

    #ifdef DEBUG
    //Log query
        logger->trace(query, 0);
    #endif

    int fail = sqlite3_open_v2(db, &dbase, SQLITE_OPEN_READWRITE, NULL);
    queryResult=SQLITE_ERROR; //Default value

    if(fail) {
    //Try to create file on fail and try again
        createFile(fail, db);
    }

    if(!fail) {
        sqlite3_prepare_v2(dbase, query, -1, &res, 0);

        //Execute query
        queryResult = sqlite3_step(res);
        //sqlite3_close_v2(dbase);

        va_end(args);

        //Checks return. Halts and logs on error.
        if(queryResult != SQLITE_OK && queryResult < 100) {
        //Result not SQLITE_OK, SQLITE_DONE, or SQLITE_ROW
            delete [] query;
            query = new char[500]; //reusing variable
            sprintf(query, "Unexpected database operation. SQLite result code %d -- %s", (int)queryResult, sqlite3_errmsg(dbase));
            logger->error(query);
        }
    } else {
        delete [] query;
        query = new char[60]; //reusing variable
        sprintf(query, "File %s does not exist. Fatal error.", db);
        logger->error(query);
    }

    delete [] query;
    query = NULL;

    return queryResult;
}

void sqliteDatabase::createFile(int & fail, const char * db)
{
            #ifdef ASSERTION_TEST
                assert(strcmp(db, data_db) != 0);
            #endif

        if(strcmp(db, data_db) != 0) {
        //Don't create the game data database
        //TODO: write test for this
            fstream file;

            //Touch file
            file.open(db, fstream::in | fstream::out | fstream::trunc);
            file.close();

            //Run query again
            fail = sqlite3_open_v2(db, &dbase, SQLITE_OPEN_READWRITE, NULL);

            if(!fail) {
            //Log
                sprintf(logBuffer, "File %s created.", db);
                logger->info(logBuffer);
            }
        }else{
        //Game database unavailable -- bailing
        //TODO: test
                logger->warn(query);

                sprintf(logBuffer, "%s failed to open!", db);
                logger->error(logBuffer);
        }
}



/********************
 Null Object
********************/

void nullDatabase::log()
{
    logger=locator::getLogging();
    logger->warn("Register service, using nullDatabase object.");
}
