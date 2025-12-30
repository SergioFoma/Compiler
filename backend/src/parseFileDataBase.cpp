#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "parseFileDataBase.h"
#include "paint.h"
#include "mathOperatorsInfo.h"
#include "myStringFunction.h"

const size_t nilLen = strlen( "nil" );
const size_t startSizeForVariable = 7;

#define FILE_WITH_TREE "commonFiles/AST.txt"     // hardcoding, because the user should not know about the intermediate files.
#define ZERO_LABEL  -1

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
        colorPrintf( NOMODE, RED, "\nError of get information from file\n" );
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
        return dataBaseFromFile;
    }

    FILE* fileForReading = fopen( nameOfFile, "r" );
    if( fileForReading == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        free( nameOfFile );
        return dataBaseFromFile;
    }

    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForReading, nameOfFile );
    if( statusOfReadFromFile != correct ){
        free( nameOfFile );
        return dataBaseFromFile;
    }

    *fileWithBuffer = fileForReading;
    free( nameOfFile );
    return dataBaseFromFile;
}

expertSystemErrors createTreeFromFile( tree_t* tree ){
    assert( tree );

    FILE* fileForDataBase = fopen( FILE_WITH_TREE, "r" );
    if( fileForDataBase == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ERROR_WITH_FILE;
    }

    bufferInformation dataBaseFromFile = {};
    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForDataBase, FILE_WITH_TREE );
    if( statusOfReadFromFile != correct ){
        return ERROR_WITH_FILE;
    }

    char* ptrOnBuffer = dataBaseFromFile.buffer;
    tree->rootTree = createNodeFromFile( &ptrOnBuffer );

    fclose( fileForDataBase );
    destroyBufferInformation( &dataBaseFromFile );

    colorPrintf( NOMODE, GREEN, "Expert system successful get data from your file\n" );
    return CORRECT_WORK;
}


node_t* createNodeFromFile( char** ptrOnSymbolInPosition ){
    if( ptrOnSymbolInPosition == NULL || *( ptrOnSymbolInPosition) == NULL ){
        return NULL;
    }

    cleanLineWithCode( ptrOnSymbolInPosition );

    if( **ptrOnSymbolInPosition == '(' ){
        ++(*ptrOnSymbolInPosition);
        node_t* newNode = NULL;
        buildNewNode( &newNode, ptrOnSymbolInPosition );
        newNode->left = createNodeFromFile( ptrOnSymbolInPosition );
        newNode->right = createNodeFromFile( ptrOnSymbolInPosition );
        ++(*ptrOnSymbolInPosition);
        return newNode;
    }
    else if( strncmp( *ptrOnSymbolInPosition, "nil", nilLen ) == 0 ){
        *ptrOnSymbolInPosition += nilLen;
        return NULL;
    }

    return NULL;
}

expertSystemErrors buildNewNode( node_t** node, char** ptrOnSymbolInPosition ){
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

    if( initializationVariableNode( node, ptrOnSymbolInPosition ) ){
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

bool initializationVariableNode( node_t** node, char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( node );

    if( isalpha( **ptrOnSymbolInPosition ) || **ptrOnSymbolInPosition == '_' ){
        char* lineWithVar = NULL;
        size_t lineLen = readingVariable( &lineWithVar, ptrOnSymbolInPosition );
        bool statusOfSearching = appendOldVariableInTree( node, ptrOnSymbolInPosition, lineWithVar, lineLen );
        if( !statusOfSearching ){
            (*node) = makeNodeWithNewVariable( lineWithVar, ptrOnSymbolInPosition, lineLen, infoForVarArray.freeIndexNow );
        }

        return true;
    }

    return false;

}

bool appendOldVariableInTree( node_t** node, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( lineWithVar );

    size_t varIndex = 0;
    for( varIndex = 0 ;varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( arrayWithVariables[ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithVariables[ varIndex ].nameOfVariable ) == 0 ){
            treeElem_t data = {};
            data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
            initNode( node, VARIABLE, data );
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

    arrayWithVariables[ infoForVarArray.freeIndexNow ] = { lineWithVar , infoForVarArray.freeIndexNow, ZERO_LABEL };
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
