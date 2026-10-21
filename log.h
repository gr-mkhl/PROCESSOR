#ifndef _LOG_H_
#define _LOG_H_

#define LOGGING
#include <stdio.h>
#include <time.h>

#ifdef LOGGING
    #define ON_LOG(...) __VA_ARGS__
#else
    #define ON_LOG(...)
#endif

#define LOG_FILENAME "LOGGER.log"
extern FILE* LOG_FILE;

void StartLog( const char* filename, const char* func, int line );
#define STARTLOG() StartLog(__FILE__, __func__, __LINE__);
char* FillTime( void );
void EndLog( const char* filename, const char* func, int line );
#define ENDLOG() EndLog(__FILE__, __func__, __LINE__);

#define LOG(...) do                                                      \
{                                                                        \
    fprintf(LOG_FILE, "%s: <", FillTime());                              \
    fprintf(LOG_FILE, __VA_ARGS__);                                      \
    fprintf(LOG_FILE, "> in %s:%s:%d\n", __FILE__, __func__, __LINE__);  \
} while (0);
#endif
