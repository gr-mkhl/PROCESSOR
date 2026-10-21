#include "log.h"

FILE* LOG_FILE = NULL;
char time_buffer[20] = {};


void StartLog( const char* filename, const char* func, int line )
{
    LOG_FILE = fopen(LOG_FILENAME, "w");

    fprintf(LOG_FILE, "%s: LOG_FILE opened in %s:%s:%d\n", FillTime(), filename, func, line);
}

char* FillTime( void )
{
    time_t now = time(NULL);

    struct tm* time_struct = localtime(&now);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", time_struct);

    return time_buffer;
}

void EndLog( const char* filename, const char* func, int line )
{
    fprintf(LOG_FILE, "%s: LOG_FILE closed in %s:%s:%d\n", FillTime(), filename, func, line);

    fclose(LOG_FILE);
}
