#include "../includes/logging.hpp"

static void formatText(const char * message, const char * severityBuffer);
static void write(const char * formatText, ...);

ILogging::~ILogging()
{
}

logging::logging()
{
    textBuffer = new char[512];
}

logging::~logging()
{
    delete [] textBuffer;
}

void logging::info(const char * format, ...)
/*Important business process has finished (use sparingly, since it can bog down
  efficiency if everything is logged).
  Also: Actions that change the state of the application significantly (i.e
  database update, loading game, etc.)*/
{
    va_list args;
    va_start (args, format);
    vsprintf (textBuffer, format, args);
    formatText(textBuffer, "");
    va_end (args);
}

void logging::trace(const char * format, ...)
//Detailed information, intended for development, removed in production.
{
    #ifdef DEBUG
        //Traces are only used in debug mode
        va_list args;
        va_start (args, format);
        vsprintf (textBuffer, format, args);
        formatText(textBuffer, "[TRACE] ");
        va_end (args);
    #endif
}

void logging::debug(const char * format, ...)
//Level used by developer. Used to debug.
{
    va_list args;
    va_start (args, format);
    vsprintf (textBuffer, format, args);

    #ifdef DEBUG
        formatText(textBuffer, "[DEBUG] ");
    #else
        //Level raises to warning if not in debug mode
        formatText(textBuffer, "[WARNING] ");
    #endif

    va_end (args);
}

void logging::attn(const char * format, ...)
/*Warn level 1 (Attention): Potential problems and suggestions (i.e. "Application running in development mode").
  Application can tolerate warning levels but they should always be justified and examined.*/
{
    va_list args;
    va_start (args, format);
    vsprintf (textBuffer, format, args);
    formatText(textBuffer, "[ATTENTION] ");
    va_end (args);
}

void logging::warn(const char * format, ...)
/*Warn level 2 (WARNING): Obvious problems where work-arounds exist (i.e. "Current data unavailable, using
  cached values"). Application can tolerate warning levels but they should always be justified and examined.*/
{
    va_list args;
    va_start (args, format);
    vsprintf (textBuffer, format, args);
    formatText(textBuffer, "[WARNING] ");
    va_end (args);
}

void logging::error(const char * format, ...)
/*Something terrible went wrong that must be investigated immediately (i.e. database unavailable, etc.).
  These are not tolerated.*/
{
    va_list args;
    va_start (args, format);
    vsprintf (textBuffer, format, args);
    formatText(textBuffer, "[ERROR] ");
    va_end (args);
    exit(EXIT_FAILURE);
}

static void formatText(const char * message, const char * severityBuffer)
{
    static char timeBuffer[26];

    //Get DateTime
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    //Output message
    strftime(timeBuffer, 26, "%a %b %d %X %Y\n", timeinfo); //Thu Aug 23 14:55:02 2001
    write("%s%s : %s", severityBuffer, message, timeBuffer);
}

static void write(const char * formatText, ...)
{
    static FILE * file = fopen("log.txt", "w"); //Write mode -- deletes previous file

    file = fopen("log.txt", "a"); //Append mode

    va_list args;
    va_start (args, formatText);
    vfprintf (file, formatText, args);
    va_end (args);

    fclose(file);
}
