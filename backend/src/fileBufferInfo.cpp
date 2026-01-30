#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "fileBufferInfo.h"
#include "myStringFunction.h"

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile ){
    assert( bufferFromFile );
    assert( nameOfFile );

    if( myFile == NULL ){
        return fileOpenErr;
    }

    getFileSize( bufferFromFile, nameOfFile );
    bufferFromFile->buffer = (char*)calloc( bufferFromFile->fileSize + 1, sizeof( char ) );
    if( bufferFromFile->buffer == NULL ){
        return memoryErr;
    }
    bufferFromFile->bufferSize = fread( bufferFromFile->buffer, sizeof( char ), bufferFromFile->fileSize, myFile );
    if( bufferFromFile->bufferSize == 0 ){
        return fileOpenErr;
    }

    (bufferFromFile->buffer)[ bufferFromFile->fileSize ]= '\0';
    (bufferFromFile->buffer)[ bufferFromFile->bufferSize ] = '\0';

    return correct;
}

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile ){
    assert( bufferFromFile != NULL );
    assert( nameOfFile != NULL );

    struct stat fileText;
    int status = stat( nameOfFile, &fileText );
    if( status == -1 ){
        return ;
    }

    bufferFromFile->fileSize = fileText.st_size;
}

void destroyBufferInformation( bufferInformation *bufferFromFile ){
    if( bufferFromFile == NULL ){
        return ;
    }

    free( bufferFromFile->buffer );
    bufferFromFile->bufferSize = 0;
    bufferFromFile->fileSize = 0;
    bufferFromFile = NULL;
}

bufferInformation getBufferFromFile( FILE** fileWithBuffer ){

    bufferInformation dataBaseFromFile = {};
    char* nameOfFile = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFile, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        free( nameOfFile );
        dataBaseFromFile.typeOfErr = correct;
        return dataBaseFromFile;
    }

    FILE* fileForReading = fopen( nameOfFile, "r" );
    if( fileForReading == NULL ){
        free( nameOfFile );
        dataBaseFromFile.typeOfErr = fileOpenErr;
        return dataBaseFromFile;
    }

    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForReading, nameOfFile );
    if( statusOfReadFromFile != correct ){
        free( nameOfFile );
        dataBaseFromFile.typeOfErr = statusOfReadFromFile;
        return dataBaseFromFile;
    }

    *fileWithBuffer = fileForReading;
    free( nameOfFile );
    return dataBaseFromFile;
}
