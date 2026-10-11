#include "file_work.h"

void PrepareFileForSorting( const char* filename, struct FileInfo* file )
{
    assert(file);
    assert(filename);

    SetFileName(file, filename);
    PrepareBuffer(file);
    ReadFromFile(file);
    CountStrings(file);
    SplitStrings(file);

    return;
}

void SetFileName( struct FileInfo* file, const char* filename )
{
    assert(file);
    assert(filename);

    char* set_file_status = strcpy(file->FileName, filename);
    assert(set_file_status);

    return;
}

void PrepareBuffer( struct FileInfo* file )
{
    assert(file);

    struct stat my_file = {};

    int stat_status = stat(file->FileName, &my_file);
    assert(stat_status != -1);

    file->FileSize = my_file.st_size;

    file->Buffer = (char*)calloc(file->FileSize + 1, sizeof(char));
    assert(file->Buffer != NULL);

    file->Buffer[file->FileSize] = '\0';

    return;
}

void ReadFromFile( struct FileInfo* file )
{
    assert(file);

    file->FileStream = open(file->FileName, O_RDONLY);
    assert(file->FileStream != -1);

    file->TextSize = read(file->FileStream, file->Buffer, (unsigned int)file->FileSize);
    assert(file->TextSize > 0);

    int close_status = close(file->FileStream);
    assert(close_status != -1);

    file->Buffer[file->TextSize] = '\0';

    return;
}

void CountStrings( struct FileInfo* file )
{
    assert(file);
    assert(file->Buffer);

    char* buffer_ptr = file->Buffer;

    size_t i = 0, n_strings = 0;

    while (buffer_ptr[i] != '\0')
    {
        if (buffer_ptr[i] == '\n')
        {
            buffer_ptr[i] = '\0';
            n_strings++;
        }
        i++;
    }
    file->NumStrings = n_strings;

    return;
}

void SplitStrings( struct FileInfo* file )
{
    assert(file);

    char** index_ptr = (char**)calloc(file->NumStrings, sizeof(char*));
    assert(index_ptr);

    index_ptr[0] = file->Buffer;
    char* buffer_ptr = file->Buffer;
    size_t n_strings = file->NumStrings;

    size_t str_index = 1;
    size_t i = 0;
    while (str_index < n_strings)
    {
        if (buffer_ptr[i] == '\0')
            index_ptr[str_index++] = &buffer_ptr[i + 1];
        i++;
    }

    file->Index = index_ptr;

    return;
}
