#ifndef H_PARSE_FILE_DATA_BASE
#define H_PARSE_FILE_DATA_BASE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

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

expertSystemErrors createTreeFromFile( tree_t* tree );

node_t* createNodeFromFile( char** ptrOnSymbolInPosition );

expertSystemErrors buildNewNode( node_t** node, char** ptrOnSymbolInPosition );

bool initializationNumberNode( node_t** node, char** ptrOnSymbolInPosition );

bool initializationVariableNode( node_t** node, char** ptrOnSymbolInPosition );

bool appendOldVariableInTree( node_t** node, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen );

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex );

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition );

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine );

void cleanLineWithCode( char** ptrOnSymbolInPosition );

#endif

