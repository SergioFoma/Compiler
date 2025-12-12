#include "stringFunction.h"
#include "paint.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

const int sortByFirst  = 0; // Sort by first letter
const int sortByLast = 1; // Sort by last letter


// Functions
errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile ){
    assert( bufferFromFile != NULL );
    assert( nameOfFile != NULL );

    if( myFile == NULL ){
        fclose( myFile );
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

errorCode initStringInformation( strInformation *stringFromFile ){
    assert( stringFromFile != NULL );

    stringFromFile->arrayOfStr = NULL;
    stringFromFile->arraySize = 0;
    return correct;
}

bool splitToLines( bufferInformation *bufferFromFile, strInformation *stringFromFile, FILE* myFile ){
    assert( bufferFromFile != NULL );
    assert( stringFromFile != NULL );
    assert( myFile != NULL );

    stringFromFile->arraySize = getSizeStrArray( bufferFromFile, '\n' );
    (stringFromFile->arrayOfStr) = (char**)calloc( stringFromFile->arraySize, sizeof( char* ) );
    if ( (stringFromFile->arrayOfStr) == NULL ){
        printfError("\n\nMemory error\n\n");
        fclose( myFile );
        return false;
    }
    colorPrintf(NOMODE, YELLOW, "Count of str: %u\n", stringFromFile->arraySize );
    getArrayOfStr( stringFromFile, bufferFromFile, '\0' );

    return true;
}


size_t getSizeStrArray( bufferInformation *bufferFromFile, char symbol ){
    assert( bufferFromFile != NULL );

    size_t arraySize = 1, sizeBuffer = 0;
    for( ; sizeBuffer < (bufferFromFile->bufferSize); sizeBuffer++){
        if ( (bufferFromFile->buffer)[sizeBuffer] == symbol ){
            (bufferFromFile->buffer)[sizeBuffer] = '\0';
            ++arraySize;
        }
    }
    return arraySize;
}
void getArrayOfStr( strInformation *stringFromFile, bufferInformation *bufferFromFile, char symbol ) {
    assert( stringFromFile != NULL );
    assert( bufferFromFile != NULL );

    size_t bufferIndex = 0, arrayStrIndex = 1;
    (stringFromFile->arrayOfStr)[0] = (bufferFromFile->buffer);
    for( ; bufferIndex < (bufferFromFile->bufferSize); bufferIndex++ ){
        if ( (bufferFromFile->buffer)[bufferIndex] == symbol ) {
            (stringFromFile->arrayOfStr)[arrayStrIndex] = ( (bufferFromFile->buffer) + bufferIndex + 1);
            ++arrayStrIndex;
        }
    }
}


void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile ){
    assert( bufferFromFile != NULL );
    assert( nameOfFile != NULL );

    struct stat fileText;
    int status = stat( nameOfFile, &fileText );

    colorPrintf(NOMODE, YELLOW, "Status of reading: %d\n", status );
    bufferFromFile->fileSize = fileText.st_size;
}



void getOriginalText( bufferInformation *bufferFromFile ){
    assert( bufferFromFile != NULL );

    for( size_t bufferIndex = 0; bufferIndex < (bufferFromFile->bufferSize); bufferIndex++ ){
        if( (bufferFromFile->buffer)[bufferIndex] == '\0' ){
            (bufferFromFile->buffer)[bufferIndex] = '\n';
        }
    }

    (bufferFromFile->buffer)[ (bufferFromFile->bufferSize)] = '\0';
}


void destroyBufferInformation( bufferInformation *bufferFromFile ){
    free( bufferFromFile->buffer );
    bufferFromFile->buffer = NULL;
    bufferFromFile->bufferSize = 0;
    bufferFromFile->fileSize = 0;
    bufferFromFile = NULL;
}

void destroyStringInformation( strInformation *stringFromFile ){
    free( stringFromFile->arrayOfStr );
    stringFromFile->arrayOfStr = NULL;
    stringFromFile->arraySize = 0;
    stringFromFile = NULL;
}

char* cleanLine( char* lineFromText) {
    assert( lineFromText != NULL);

    size_t lineIndex = 0, sizeOfLine = strlen( lineFromText );
    while( lineIndex < sizeOfLine && isalpha( *(lineFromText + lineIndex ) ) == 0 ) {
        ++lineIndex;
    }
    return lineFromText + lineIndex;
}

ssize_t getlineWrapper( char** line, size_t* n, FILE* stream ) {

    ssize_t sizeOfLine = getline( line, n, stream );
    if( sizeOfLine == -1 ){
        return -1;
    }

    if( (*line)[ sizeOfLine - 1 ] == '\n' ){
        (*line)[ sizeOfLine - 1 ] = '\0';
    }

    return sizeOfLine;
}
