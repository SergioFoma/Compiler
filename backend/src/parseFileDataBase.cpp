#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "parseFileDataBase.h"
#include "functionsData.h"
#include "paint.h"
#include "mathOperatorsInfo.h"
#include "myStringFunction.h"
#include "fileBufferInfo.h"

const size_t nilLen = strlen( "nil" );
const size_t funcLen = sizeof( "FUNC" ) - 1;
const size_t startSizeForVariable = 7;
const size_t zeroPosition = 0;
const size_t specialIndex = 52;

expertSystemErrors createTreeFromFile( tree_t* tree, const char* fileNameWithAST ){
    assert( tree );

    FILE* fileForDataBase = fopen( fileNameWithAST, "r" );
    if( fileForDataBase == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ERROR_WITH_FILE;
    }

    bufferInformation dataBaseFromFile = {};
    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForDataBase, fileNameWithAST );
    if( statusOfReadFromFile != correct ){
        return ERROR_WITH_FILE;
    }

    char* ptrOnBuffer = dataBaseFromFile.buffer;
    bool doNeedToCreateNodeWithVar = true;
    tree->rootTree = createNodeFromFile( &ptrOnBuffer, doNeedToCreateNodeWithVar );

    fclose( fileForDataBase );
    destroyBufferInformation( &dataBaseFromFile );

    colorPrintf( NOMODE, GREEN, "Expert system successful get data from your file\n" );
    return CORRECT_WORK;
}


node_t* createNodeFromFile( char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar ){
    if( ptrOnSymbolInPosition == NULL || *( ptrOnSymbolInPosition) == NULL ){
        return NULL;
    }

    cleanLineWithCode( ptrOnSymbolInPosition );

    if( **ptrOnSymbolInPosition == '(' ){
        ++(*ptrOnSymbolInPosition);
        node_t* newNode = NULL;

        buildNewNode( &newNode, ptrOnSymbolInPosition, doNeedToCreateNodeWithVar );

        if( newNode->nodeValueType == STATEMENT && newNode->data.statement == FUNC ){
            searchRightChildOfFunction( ptrOnSymbolInPosition );
        }

        newNode->left = createNodeFromFile( ptrOnSymbolInPosition, doNeedToCreateNodeWithVar );
        newNode->right = createNodeFromFile( ptrOnSymbolInPosition, doNeedToCreateNodeWithVar );

        if( newNode->nodeValueType == STATEMENT && newNode->data.statement == FUNC && newNode->right ){
            functionInformations.currentFunctionIndex = 0;
        }

        ++(*ptrOnSymbolInPosition);
        cleanLineWithCode( ptrOnSymbolInPosition );
        return newNode;
    }
    else if( strncmp( *ptrOnSymbolInPosition, "nil", nilLen ) == 0 ){
        *ptrOnSymbolInPosition += nilLen;
        cleanLineWithCode( ptrOnSymbolInPosition );
        return NULL;
    }

    return NULL;
}

void searchRightChildOfFunction( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    isEnoughSizeForFunctionArray();

    char* copySymbolForFunction = *ptrOnSymbolInPosition;
    bool doNeedToCreateNodeWithVar = false;

    node_t* left = createNodeFromFile( &copySymbolForFunction, doNeedToCreateNodeWithVar );
    node_t* right = createNodeFromFile( &copySymbolForFunction, doNeedToCreateNodeWithVar );

    if( right ){
        functionInformations.currentFunctionIndex = functionInformations.countOfFunction;
        ++(functionInformations.countOfFunction);
    }
    destroyNode( left );
    destroyNode( right );
}

expertSystemErrors buildNewNode( node_t** node, char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( node );

    treeElem_t data = {};

    for( size_t expressionIndex = 0; expressionIndex < sizeOfExpressionArray; expressionIndex++ ){
        informationWithExpressions expressionStruct = arrayWithExpressions[ expressionIndex ];
        size_t lineLen = strlen( expressionStruct.viewOfExpressionInAST );
        if( strncmp( *ptrOnSymbolInPosition, expressionStruct.viewOfExpressionInAST, lineLen ) == 0 ){
            data.expressionOperator = expressionStruct.expressionOperator;
            initNode( node, EXPRESSION, data );
            *ptrOnSymbolInPosition += lineLen;
            return CORRECT_WORK;
        }
    }

    for( size_t mathIndex = 0; mathIndex < sizeOfMathArray; mathIndex++ ){
        informationWithMathOperators mathStruct = arrayWithMathInfo[ mathIndex ];
        size_t lineLen = strlen( mathStruct.viewOfMathOperationInFile );
        if( strncmp( *ptrOnSymbolInPosition, mathStruct.viewOfMathOperationInFile, lineLen  ) == 0 ){
            data.mathOperation = mathStruct.mathOperation;
            initNode( node, OPERATOR, data );
            *ptrOnSymbolInPosition += lineLen;
            return CORRECT_WORK;
        }

    }

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        informationWithStatements statementStruct = arrayWithStatements[ statementIndex ];
        size_t lineLen = strlen( statementStruct.viewOfStatementInAST );
        if( strncmp( *ptrOnSymbolInPosition, statementStruct.viewOfStatementInAST, lineLen ) == 0 ){
            data.statement = statementStruct.statement;
            initNode( node, STATEMENT, data );
            *ptrOnSymbolInPosition += lineLen;
            return CORRECT_WORK;
        }
    }

    if( initializationNumberNode( node, ptrOnSymbolInPosition ) ){
        return CORRECT_WORK;
    }

    if( initializationVariableNode( node, ptrOnSymbolInPosition, doNeedToCreateNodeWithVar ) ){
        return CORRECT_WORK;
    }

    return CORRECT_WORK;
}

bool initializationNumberNode( node_t** node, char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( node );

    if( '0' <= **ptrOnSymbolInPosition && **ptrOnSymbolInPosition <= '9' ){
        double value = 0;
        do{
            value = value * 10 + ( **ptrOnSymbolInPosition - '0' );
            ++(*ptrOnSymbolInPosition);
        }while( '0' <= **ptrOnSymbolInPosition && **ptrOnSymbolInPosition <= '9' );

        treeElem_t data = {};
        data.number = value;
        initNode( node, NUMBER, data );
        return true;
    }

    return false;
}

bool initializationVariableNode( node_t** node, char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( node );

    if( isalpha( **ptrOnSymbolInPosition ) || **ptrOnSymbolInPosition == '_' ){
        char* lineWithVar = NULL;
        size_t lineLen = readingVariable( &lineWithVar, ptrOnSymbolInPosition );

        if( initializationDummyVariable( node, ptrOnSymbolInPosition, doNeedToCreateNodeWithVar, lineLen ) ){
            free( lineWithVar );
            return true;
        }

        bool statusOfSearching = appendOldVariableInTree( node, ptrOnSymbolInPosition, lineWithVar, lineLen );
        if( !statusOfSearching ){
            (*node) = makeNodeWithNewVariable( lineWithVar, ptrOnSymbolInPosition, lineLen );
        }

        return true;
    }

    return false;

}

bool initializationDummyVariable( node_t** node, char** ptrOnSymbolInPosition, bool doNeedToCreateNodeWithVar, size_t lineLen ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( node );

    if( doNeedToCreateNodeWithVar == false ){
        *ptrOnSymbolInPosition += lineLen;
        treeElem_t data = {};
        data.variableIndexInArray = specialIndex;
        initNode( node, VARIABLE, data );
        return true;
    }

    return false;
}

bool appendOldVariableInTree( node_t** node, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( lineWithVar );

    size_t varIndex = 0;
    for( varIndex = 0; varIndex < arrayWithSizeOfEveryFunctions[ functionInformations.currentFunctionIndex ].freeIndexNow; varIndex++ ){
        if( arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ][ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ][ varIndex ].nameOfVariable ) == 0 ){

            size_t* freeIndexNow = &(arrayWithSizeOfEveryFunctions[ functionInformations.currentFunctionIndex ].freeIndexNow);
            size_t* capacity = &(arrayWithSizeOfEveryFunctions[ functionInformations.currentFunctionIndex ].capacity);

            if( *freeIndexNow == *capacity - 1){
                (*capacity) *= 2;
                arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ] =
                (informationWithVariables*)realloc( arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ],
                (*capacity) * sizeof( informationWithVariables ) );
            }

            treeElem_t data = {};
            data.variableIndexInArray = arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ][ varIndex ].variableIndexInArray;
            initNode( node, VARIABLE, data );
            arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ][ *freeIndexNow ] = { lineWithVar , data.variableIndexInArray, *node };

            ++(*freeIndexNow);
            *ptrOnSymbolInPosition += lineLen;
            return true;
        }
    }
    return false;
}

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen ){
    assert( lineWithVar );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    size_t* freeIndexNow = &(arrayWithSizeOfEveryFunctions[ functionInformations.currentFunctionIndex ].freeIndexNow);
    size_t* capacity = &(arrayWithSizeOfEveryFunctions[ functionInformations.currentFunctionIndex ].capacity);
    size_t* maxIndex = &(arrayWithSizeOfEveryFunctions[ functionInformations.currentFunctionIndex ].maximumIndexOfVariable);

    if( *freeIndexNow == *capacity - 1){
            (*capacity) *= 2;
            arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ] =
            (informationWithVariables*)realloc( arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ],
            (*capacity) * sizeof( informationWithVariables ) );
        }

    treeElem_t data = {};
    data.variableIndexInArray = *maxIndex;
    node_t* nodeWithVar = NULL;
    initNode( &nodeWithVar, VARIABLE, data );
    arrayWithInfoForFunctions[ functionInformations.currentFunctionIndex ][ *freeIndexNow ] = { lineWithVar , data.variableIndexInArray, nodeWithVar };
    ++(*freeIndexNow);
    ++(*maxIndex);
    *ptrOnSymbolInPosition += lineLen;

    cleanLineWithCode( ptrOnSymbolInPosition );
    return nodeWithVar;
}

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition ){
    assert( lineWithVariable );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForVariable;
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

void cleanLineWithCode( char** ptrOnSymbolInPostion ){
    while( **ptrOnSymbolInPostion == '\n' || **ptrOnSymbolInPostion == ' ' ){
        ++(*ptrOnSymbolInPostion);
    }
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
