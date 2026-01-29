#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexer.h"
#include "mathOperatorsInfo.h"
#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"

const size_t startSizeForWord = 7;
const size_t startSizeForTokens = 20;
const char* commentBegin = "##!";
const char* commentEnd = "!##";
const size_t commentLen = sizeof( commentBegin ) - 1;

void initializationTokens( infoForCreateTree* infoForTree ){
    infoForTree->countOfTokens = startSizeForTokens;
    infoForTree->tokens= ( node_t** )calloc( infoForTree->countOfTokens, sizeof( node_t* ) );
    size_t startIndex = 0;
    initializationNodeInArray( infoForTree, startIndex );

}

void initializationNodeInArray( infoForCreateTree* infoForTree, size_t startIndex ){
    assert( infoForTree );

    treeElem_t data = {};
    data.statement = NO_TYPE;
    for( ; startIndex < infoForTree->countOfTokens; startIndex++ ){
        initNode( infoForTree->tokens + startIndex, STATEMENT, data );
    }
}

void lexAnalysis( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    while( **symbol != '\0' ){
        if( infoForTree->freeIndexNow == ( infoForTree->countOfTokens - 1) ){
            infoForTree->countOfTokens *= 2;
            infoForTree->tokens = ( node_t** )realloc( infoForTree->tokens, sizeof( node_t* ) * infoForTree->countOfTokens );
            initializationNodeInArray( infoForTree, infoForTree->freeIndexNow + 1 );
        }

        checkingOnComment( symbol );

        if( tryInitializationExpressionNode( symbol, infoForTree ) == true ){
            continue;
        }

        if( tryInitializationStatement( symbol, infoForTree ) == true ){
            continue;
        }

        if( tryInitializationMathOperator( symbol, infoForTree ) == true ){
            continue;
        }


        if( tryInitializationNumber( symbol, infoForTree ) == true ){
            continue;
        }

        if( tryInitializationVariable( symbol, infoForTree ) == true ){
            continue;

        }

        if( isspace( **symbol ) ){
            cleanLineWithCode( symbol );
            continue;
        }

        colorPrintf( NOMODE, RED, "\n\nERROR OF LEX ANALYSIS\n\nNOT UNDEFINED SYMBOL  '%c' \n",**symbol );
        exit( 0 );
    }
}

void checkingOnComment( char** symbol ){
    assert( symbol );
    assert( *symbol );

    if( strncmp( *symbol, commentBegin, commentLen ) == 0 ){
        *symbol += commentLen;
        while( strncmp( *symbol, commentEnd, commentLen ) != 0 ){
            ++( *symbol );
        }
        *symbol += commentLen;
    }
}

bool tryInitializationExpressionNode( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        const char* nameOfOperator = arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile;
        size_t lenOfOperator = strlen( arrayWithExpressions[ expressionIndex ].viewOfExpressionOperatorInFile );

        if( strncmp( *symbol, nameOfOperator, lenOfOperator ) == 0 ){

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = EXPRESSION;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.expressionOperator = arrayWithExpressions[ expressionIndex ].expressionOperator;
            (*symbol) += lenOfOperator;
            ++( infoForTree->freeIndexNow );

            return true;
        }
    }

    return false;
}

bool tryInitializationStatement( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        const char* nameOfStatement = arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        size_t lenOfStatement = strlen( arrayWithStatements[ statementIndex ].viewOfStatementInFile );

        if( strncmp( *symbol, nameOfStatement, lenOfStatement ) == 0 ){

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = STATEMENT;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.statement = arrayWithStatements[ statementIndex ].statement;
            (*symbol) += lenOfStatement;
            ++( infoForTree->freeIndexNow );

            return true;
        }
    }

    return false;
}

bool tryInitializationMathOperator( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    for( size_t mathIndex = 0; mathIndex < sizeOfStatementArray; mathIndex++ ){
        const char* nameOfMath = arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile;
        size_t lenOfMath = strlen( arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile );

        if( strncmp( *symbol, nameOfMath, lenOfMath ) == 0 ){

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = OPERATOR;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.mathOperation = arrayWithMathInfo[ mathIndex ].mathOperation;
            (*symbol) += lenOfMath;
            ++( infoForTree->freeIndexNow );

            return true;
        }
    }

    return false;
}

bool tryInitializationNumber( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    if( '0' <= **symbol && **symbol <= '9' ){
        double value = 0;
        do{
            value = value * 10 + ( **symbol - '0' );
            ++(*symbol);
        }while( '0' <= **symbol && **symbol <= '9' );

        (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = NUMBER;
        (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.number = value;
        ++( infoForTree->freeIndexNow );

        return true;
    }

    return false;
}

bool tryInitializationVariable( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    if( isalpha( **symbol ) || **symbol == '_' ){
        char* lineWithVar = NULL;
        size_t lineLen = readingVariable( &lineWithVar, symbol );
        bool statusOfSearching = changTypeOfNodeOnVariableNode( infoForTree, symbol, lineWithVar, lineLen );
        if( !statusOfSearching ){
            free( ( infoForTree->tokens )[ infoForTree->freeIndexNow ] );
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ] = makeNodeWithNewVariable( lineWithVar, symbol, lineLen, infoForVarArray.freeIndexNow );
        }
        ++( infoForTree->freeIndexNow );

        return true;
    }

    return false;
}

bool changTypeOfNodeOnVariableNode( infoForCreateTree* infoForTree, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen ){
    assert( infoForTree );
    assert( lineWithVar );

    size_t varIndex = 0;
    for( varIndex = 0 ;varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( arrayWithVariables[ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithVariables[ varIndex ].nameOfVariable ) == 0 ){
            treeElem_t data = {};
            data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ]->nodeValueType = VARIABLE;
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ]->data = data;
            *ptrOnSymbolInPosition += lineLen;
            free( lineWithVar );
            return true;
        }
    }
    return false;
}

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex ){
    assert( lineWithVar );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if( infoForVarArray.freeIndexNow == infoForVarArray.capacity - 1){
            infoForVarArray.capacity *= 2;
            arrayWithVariables = (informationWithVariables*)realloc( arrayWithVariables, infoForVarArray.capacity * sizeof( informationWithVariables ) );
            arrayWithVariableValue = (double*)realloc( arrayWithVariableValue, infoForVarArray.capacity * sizeof( double ) );
        }

    arrayWithVariables[ infoForVarArray.freeIndexNow ] = { lineWithVar , infoForVarArray.freeIndexNow  };
    treeElem_t data = {};
    data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
    node_t* nodeWithVar = NULL;
    initNode( &nodeWithVar, VARIABLE, data );
    ++(infoForVarArray.freeIndexNow);
    *ptrOnSymbolInPosition += lineLen;

    arrayWithVariableValue[  infoForVarArray.freeIndexNow ] = 0;        // initialization
    cleanLineWithCode( ptrOnSymbolInPosition );
    return nodeWithVar;
}

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition ){
    assert( lineWithWord );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithWord = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( isalpha( (*ptrOnSymbolInPosition)[ lineIndex ] ) || (*ptrOnSymbolInPosition)[ lineIndex ] == '_' ){
            isEnoughSize( lineWithWord, &lineIndex, &sizeOfLine );
            (*lineWithWord)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithWord)[ lineIndex ] = '\0';

    cleanLineWithCode( ptrOnSymbolInPosition );
    return lineIndex;
}

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition ){
    assert( lineWithVariable );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithVariable = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( isalpha( (*ptrOnSymbolInPosition)[ lineIndex ] ) || (*ptrOnSymbolInPosition)[ lineIndex ] == '_' ||
          ( '0' <= (*ptrOnSymbolInPosition)[ lineIndex ]  && (*ptrOnSymbolInPosition)[ lineIndex ]  <= '9')  ){
            isEnoughSize( lineWithVariable, &lineIndex, &sizeOfLine );
            (*lineWithVariable)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithVariable)[ lineIndex ] = '\0';

    cleanLineWithCode( ptrOnSymbolInPosition );
    return lineIndex;

}

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine ){
    assert( lineWithWord );
    assert( lineIndex );
    assert( sizeOfLine );

    if( *lineIndex == *sizeOfLine - 1 ){
        *sizeOfLine *= 2;
        *lineWithWord = (char*)realloc( *lineWithWord, *sizeOfLine * sizeof( char ) );
    }
};

void cleanLineWithCode( char** ptrOnSymbolInPostion ){
    while( **ptrOnSymbolInPostion == '\n' || **ptrOnSymbolInPostion == ' ' ){
        ++(*ptrOnSymbolInPostion);
    }
}

void dumpLexArrayInFile( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will save info about tokens: " );

    char* nameOfFileForTokens = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForTokens, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ;
    }

    FILE* fileForTokens = fopen( nameOfFileForTokens, "w" );
    fprintf( fileForTokens, "array with tokens[ %lu ] = {\n", infoForTree->countOfTokens );

    for( size_t tokenIndex = 0; tokenIndex < infoForTree->countOfTokens; tokenIndex++ ){
        if( ( infoForTree->tokens )[ tokenIndex ] == NULL ){
            continue;
        }

        if( ( infoForTree->tokens )[ tokenIndex ]->nodeValueType == NUMBER ){
            fprintf( fileForTokens, "\t[%lu] = { type = NUMBER, val = %lg}\n", tokenIndex, ( infoForTree->tokens )[ tokenIndex ]->data.number );
        }
        else{
            fprintf( fileForTokens, "\t[%lu] = { type = %s, val = '%s'}\n",
                     tokenIndex, getStringOfValueType( ( infoForTree->tokens )[ tokenIndex] ), getStringOfNodeValue( ( infoForTree->tokens )[ tokenIndex] ) );
        }
    }
    fprintf( fileForTokens, "};\ncapacity = %lu\nfreeIndexNow = %lu\ncurrentIndex = %lu\n", infoForTree->countOfTokens, infoForTree->freeIndexNow, infoForTree->currentIndex );


    free( nameOfFileForTokens );
    fclose( fileForTokens );
}

void destroyLexArray( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t tokenIndex = 0; tokenIndex < infoForTree->countOfTokens; tokenIndex++ ){

        free( ( infoForTree->tokens )[tokenIndex ] );
    }

    free( infoForTree->tokens );
}
