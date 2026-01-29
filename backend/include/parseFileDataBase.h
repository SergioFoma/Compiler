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

struct bufferInformation {
    char* buffer;
    size_t fileSize;
    size_t bufferSize;
};

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile );

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile );

void destroyBufferInformation( bufferInformation *bufferFromFile );

bufferInformation getBufferFromFile( FILE** fileWithBuffer );

expertSystemErrors createTreeFromFile( tree_t* tree, const char* fileNameWithAST );

node_t* createNodeFromFile( char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar );

void searchRightChildOfFunction( char** ptrOnSymbolInPosition );

expertSystemErrors buildNewNode( node_t** node, char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar );

bool initializationNumberNode( node_t** node, char** ptrOnSymbolInPosition );

bool initializationVariableNode( node_t** node, char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar );

bool initializationDummyVariable( node_t** node, char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar, size_t lineLen );

bool appendOldVariableInTree( node_t** node, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen );

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen );

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition );

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine );

void cleanLineWithCode( char** ptrOnSymbolInPosition );

void destroyArrayWithVariables();
#endif

