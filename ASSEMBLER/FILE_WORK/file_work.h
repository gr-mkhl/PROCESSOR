#ifndef _FILE_WORK_
#define _FILE_WORK_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_NAME_SIZE 200
struct FileInfo
{
    int FileStream;
    size_t FileSize;
    ssize_t TextSize;
    size_t NumStrings;
    char* Buffer;
    char** Index;
    char FileName[MAX_NAME_SIZE];

};

void PrepareFileForSorting( const char* filename, struct FileInfo* file );
void SetFileName( struct FileInfo* file, const char* filename );
void PrepareBuffer( struct FileInfo* file );
void ReadFromFile( struct FileInfo* file );
void CountStrings( struct FileInfo* file );
void SplitStrings( struct FileInfo* file );

#endif
