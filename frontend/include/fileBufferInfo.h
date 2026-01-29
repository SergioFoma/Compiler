#ifndef H_FILE_BUFFER_INFO
#define H_FILE_BUFFER_INFO

#include <stdio.h>

enum errorCode {
    correct        = 0,
    fileOpenErr    = 1,
    memoryErr      = 2
};

struct bufferInformation{
    char* buffer;
    size_t fileSize;
    size_t bufferSize;
    errorCode typeOfErr;
};

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile );

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile );

void destroyBufferInformation( bufferInformation *bufferFromFile );

bufferInformation getBufferFromFile( FILE** fileWithBuffer );

#endif
