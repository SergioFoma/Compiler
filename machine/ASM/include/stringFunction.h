#ifndef H_STRINGFUNCTION
#define H_STRINGFUNCTION

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct bufferInformation{
    char* buffer;
    size_t fileSize;
    size_t bufferSize;
};

struct strInformation{
    char** arrayOfStr;
    size_t arraySize;
};

enum errorCode {
    correct        = 0,
    fileOpenErr    = 1,
    memoryErr      = 2
};

//--------------------------------------------------------------------------------------------------------
//!
//! @return true if reading from the file, allocating memory, and processing the rows were successful,
//!         otherwise false
//!
//! @brief reads a text file into an array, splits it into lines, sorts it, and writes the result to a file.
//!
//-----------------------------------------------------------------------------------------------------------
bool workWithBuffer( char* nameFileForRead, char* nameFileForWrite);

//------------------------------------------------------------------------------------------------------------
//!
//! @param[in] **arrayOfStr    **arrayOfStr - array of pointers.
//! @param[in] *bufer           *bufer - an array where the entire text file was read.
//! @param[in] bufferSize          bufferSize - text file size.
//! @param[in] symbol           symbol - the character used for splitting into lines.
//!
//! @brief retrieves the array where the entire text file was read,
//!        and splits it into lines using the specified character.
//!
//------------------------------------------------------------------------------------------------------------
void getArrayOfStr( strInformation *stringFromFile, bufferInformation *bufferFromText, char symbol );

//-----------------------------------------------------------------------------------------------------------
//!
//! @param[in] *bufer           *bufer - an array where the entire text file was read.
//! @param[in] bufferSize          bufferSize - text file size.
//! @param[in] symbol           symbol - the character used for splitting into lines.
//!
//! @return count of lines.
//!
//! @brief counts the number of lines in a file.
//!
//-------------------------------------------------------------------------------------------------------------
size_t getSizeStrArray( bufferInformation *bufferFromFile, char symbol );

//-------------------------------------------------------------------------------------------------------------
//!
//! @param[in] **arrayOfStr     **arrayOfStr - array of pointers.
//! @param[in] arrayStrSize     arrayStrSize - count of lines if arrayOfStr.
//!
//! @brief implements bubble sorting by the beginning of rows.
//!
//-------------------------------------------------------------------------------------------------------------

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile );

//---------------------------------------------------------------------------------------------------------------
//!
//! @param[in] *bufferFromFile      *bufferFromFile - an array in which \n is replaced by \0.
//! @param[in] bufferSize             bufferSize - size of bufferFromFile.
//!
//! @brief Changes all characters \n to \0 in the limit of the array.
//!
//-------------------------------------------------------------------------------------------------------------
void getOriginalText( bufferInformation *bufferFromFile );

//------------------------------------------------------------------------------------------------------------
//!
//! @param[in] *frist       *first - first const string.
//! @param[in] *second      *second - second const string.
//!
//! @return the value is less than zero if the first row is less than the second,
//!         zero if the first row is equal to the second row, otherwise positive.
//!
//! @brief compares two strings lexicographically by the beginning.
//!
//--------------------------------------------------------------------------------------------------------------


errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile );

errorCode initStringInformation( strInformation *stringFromFile );

void destroyBufferInformation( bufferInformation *bufferFromFile );

void destroyStringInformation( strInformation *stringFromFile );

bool splitToLines( bufferInformation* bufferFromFile, strInformation *stringFromFile, FILE* myFile );

char* cleanLine( char* lineFromText );
#endif

