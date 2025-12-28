#ifndef H_PARSEFILEDATABASE
#define H_PARSEFILEDATABASE

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

struct infoForCreateTree {
    node_t** tokens;
    size_t countOfTokens;
    size_t currentIndex;
    size_t freeIndexNow;
};

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile );

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile );

void destroyBufferInformation( bufferInformation *bufferFromFile );

bufferInformation getBufferFromFile( FILE** fileWithBuffer );

expertSystemErrors buildnewMathNode( node_t** node, char* nodeName );

void initializationTokens( infoForCreateTree* infoForTree );

void initializationNodeInArray( infoForCreateTree* infoForTree, size_t startIndex );

bool changTypeOfNodeOnVariableNode( infoForCreateTree* infoForTree, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen );

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree );

void lexAnalysis( char** mathExpression, infoForCreateTree* infoForTree );

void checkingOnComment( char** symbol );

bool tryInitializationExpressionNode( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationStatement( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationMathOperator( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationNumber( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationVariable( char** symbol, infoForCreateTree* infoForTree );

node_t* getGeneral( infoForCreateTree* infoForTree );

node_t* makeTreeFromOperators( infoForCreateTree* infoForTree );

node_t* getOperator( infoForCreateTree* infoForTree );

node_t* getNodeFromOperatorThatCalledOperator( infoForCreateTree* infoForTree );

node_t* getAssignment( infoForCreateTree* infoForTree );

node_t* getCondition( infoForCreateTree* infoForTree );

node_t* getCycle( infoForCreateTree* infoForTree );

node_t* getPrint( infoForCreateTree* infoForTree );

node_t* getFunctionDefinition( infoForCreateTree* infoForTree );

node_t* getFunctionDeclaration( infoForCreateTree* infoForTree );

node_t* getArgumentInFunction( infoForCreateTree* infoForTree );

node_t* getInput( infoForCreateTree* infoForTree );

const char* getEndOfAssignment();

node_t* getExpression( infoForCreateTree* infoForTree );

node_t* getElse( infoForCreateTree* infoForTree );

node_t* getLogicalAnd( infoForCreateTree* infoForTree );

node_t* getLogicalOr( infoForCreateTree* infoForTree );

node_t* getTypeOfEqual( infoForCreateTree* infoForTree );

node_t* getCompareOperations( infoForCreateTree* infoForTree );

node_t* getTerm( infoForCreateTree* infoForTree );

node_t* getPow( infoForCreateTree* infoForTree );

node_t* getFunctionWithOneArgument( infoForCreateTree* infoForTree );

node_t* getPrimaryExpression( infoForCreateTree* infoForTree );

node_t* getVariable( infoForCreateTree* infoForTree );

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex );

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition );

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition );

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine );

bool checkingOnFunction( infoForCreateTree* infoForTree );

bool checkingOnStatement( infoForCreateTree* infoForTree );

bool checkingOnExpression( infoForCreateTree* infoForTree );

node_t* getNumber( infoForCreateTree* infoForTree );

void cleanLineWithCode( char** ptrOnSymbolInPosition );

void dumpLexArrayInFile( infoForCreateTree* infoForTree );

void destroyLexArray( infoForCreateTree* infoForTree );

#endif

