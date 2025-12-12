#ifndef H_PARSE_FILE_DATA_BASE
#define H_PARSE_FILE_DATA_BASE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

enum errorCode {
    correct        = 0,
    fileOpenErr    = 1,
    memoryErr      = 2
};

enum expertSystemErrors {
    CORRECT_WORK            = 0,
    CAN_NOT_DO_ALLOCATION   = 1,
    ERROR_WITH_GETLINE      = 2,
    NOT_CORRECT_WORK        = 3,
    ERROR_OF_CLEAN          = 4,
    TREE_NULL_PTR           = 5,
    TREE_ERRORS             = 6,
    NULL_PTR_IN_FUNC        = 7,
    ERROR_WITH_FILE         = 8,
    ROOT_NULL_PTR           = 9
};

struct bufferInformation{
    char* buffer;
    size_t fileSize;
    size_t bufferSize;
};

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile );

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile );

void destroyBufferInformation( bufferInformation *bufferFromFile );

bufferInformation getBufferFromFile( FILE** fileWithBuffer );

expertSystemErrors createTreeFromFile( tree_t* tree );

node_t* createNodeFromFile( char** ptrOnSymbolInPosition );

char* readNodeNameFromFile( char** ptrOnSymbolInPosition );

expertSystemErrors buildNewNode( node_t** node, char** ptrOnSymbolInPosition );

void cleanLineWithCode( char** ptrOnSymbolInPosition );

#endif

