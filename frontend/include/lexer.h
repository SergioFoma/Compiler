#ifndef H_LEXER
#define H_LEXER

#include "tree.h"

struct infoForCreateTree {
    node_t** tokens;
    size_t countOfTokens;
    size_t currentIndex;
    size_t freeIndexNow;
};

void initializationTokens( infoForCreateTree* infoForTree );

void initializationNodeInArray( infoForCreateTree* infoForTree, size_t startIndex );

bool changTypeOfNodeOnVariableNode( infoForCreateTree* infoForTree, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen );

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex );

void lexAnalysis( char** mathExpression, infoForCreateTree* infoForTree );

void checkingOnComment( char** symbol );

bool tryInitializationExpressionNode( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationStatement( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationMathOperator( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationNumber( char** symbol, infoForCreateTree* infoForTree );

bool tryInitializationVariable( char** symbol, infoForCreateTree* infoForTree );

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition );

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition );

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine );

void cleanLineWithCode( char** ptrOnSymbolInPosition );

void dumpLexArrayInFile( infoForCreateTree* infoForTree );

void destroyLexArray( infoForCreateTree* infoForTree );


#endif
