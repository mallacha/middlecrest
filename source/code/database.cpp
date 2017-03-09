#include "../includes/database.hpp"
#include "../includes/configuration.hpp"
#include "../includes/service.hpp"

using namespace std;

#ifdef DEBUG
    static const char * data_db = "gameDataDev.db";
    static const char * save_db = "save/saveDataDev.db";
#else
    static const char * data_db = "gameData.db";
    static const char * save_db = "save/saveData.db";
#endif

/********************
 Database
********************/

sqliteDatabase::sqliteDatabase()
{
    queryResult = SQLITE_OK; //Initialize to OK
    logger = locator::getLogging();
    query = NULL;

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
        query = new char[bufferSize];
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

    sqlite3* dbase;
    int fail = sqlite3_open_v2(db, &dbase, SQLITE_OPEN_READWRITE, NULL);
    queryResult=SQLITE_ERROR; //Default value
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
            query = new char[60]; //reusing variable
            sprintf(query, "Unexpected database operation. SQLite result code %d", (int)queryResult);
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



/********************
 Null Object
********************/

void nullDatabase::log()
{
    logger=locator::getLogging();
    logger->warn("Register service, using nullDatabase object.");
}
