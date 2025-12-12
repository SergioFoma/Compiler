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

    colorPrintf( NOMODE, YELLOW, "Enter the name of file, that will used for make tree: " );

    char* nameOfFileForTree = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForTree, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ERROR_WITH_GETLINE;
    }

    FILE* fileForDataBase = fopen( nameOfFileForTree, "r" );
    if( fileForDataBase == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        free( nameOfFileForTree );
        return ERROR_WITH_FILE;
    }

    bufferInformation dataBaseFromFile = {};
    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForDataBase, nameOfFileForTree );
    if( statusOfReadFromFile != correct ){
        return ERROR_WITH_FILE;
    }

    char* ptrOnBuffer = dataBaseFromFile.buffer;
    tree->rootTree = createNodeFromFile( &ptrOnBuffer );

    fclose( fileForDataBase );
    free( nameOfFileForTree );
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

char* readNodeNameFromFile( char** ptrOnSymbolInPosition ){
    if( ptrOnSymbolInPosition == NULL || *(ptrOnSymbolInPosition) == NULL ){
        return NULL;
    }

    char* startOfLineWithNodeName = *ptrOnSymbolInPosition;
    char* endOfLineWithNodeName = strchr( startOfLineWithNodeName, ' ' );

    char* nodeName = (char*)calloc( (size_t)(endOfLineWithNodeName - startOfLineWithNodeName) + 1, sizeof( char ) );
    if( nodeName == NULL ){
        return NULL;
    }
    sscanf( startOfLineWithNodeName, "%[^ ]", nodeName );
    *ptrOnSymbolInPosition = endOfLineWithNodeName + 1;

    return nodeName;
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


    if( '0' <= **ptrOnSymbolInPosition && **ptrOnSymbolInPosition <= '9' ){
        printf( "char before = %c\n", **ptrOnSymbolInPosition );
        double value = 0;
        do{
            value = value * 10 + ( **ptrOnSymbolInPosition - '0' );
            ++(*ptrOnSymbolInPosition);
        }while( '0' <= **ptrOnSymbolInPosition && **ptrOnSymbolInPosition <= '9' );

        data.number = value;
        initNode( node, NUMBER, data );
        return CORRECT_WORK;
    }

    return CORRECT_WORK;
}

void cleanLineWithCode( char** ptrOnSymbolInPostion ){
    while( **ptrOnSymbolInPostion == '\n' || **ptrOnSymbolInPostion == ' ' ){
        ++(*ptrOnSymbolInPostion);
    }
}
