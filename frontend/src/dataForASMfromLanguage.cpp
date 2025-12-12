#include <stdio.h>

#include "dataForASMfromLanguage.h"
#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"

expertSystemErrors writeAST( tree_t* tree ){
    assert( tree );

    colorPrintf( NOMODE, YELLOW, "\nEnter the name of file were do you want save PUBG language: " );

    char* fileNameForWrite = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &fileNameForWrite, &sizeOfAllocationMemory, stdin );
    if( sizeOfLine == -1 ){
        return ERROR_WITH_GETLINE;
    }

    FILE* fileForTree = fopen( fileNameForWrite, "w" );
    if( fileForTree == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in file\n" );
        return ERROR_WITH_FILE;
    }

    dumpTreeInFile( tree, fileForTree );

    free( fileNameForWrite );

    fclose( fileForTree );
    return CORRECT_WORK;
}

expertSystemErrors writeCommandForAssemble( ){

    colorPrintf( NOMODE, YELLOW, "\nEnter the name of AST file. I will write command for assemble: " );

    char* fileNameForAssemble = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &fileNameForAssemble, &sizeOfAllocationMemory, stdin );
    if( sizeOfLine == -1 ){
        return ERROR_WITH_GETLINE;
    }

    FILE* fileWithAST = NULL;
    bufferInformation dataBaseFromFile = getBufferFromFile( &fileWithAST );
    char* ptrOnSymbolInPosition = dataBaseFromFile.buffer;

    writeCommandForASM( &ptrOnSymbolInPosition, fileWithAST );
}

void writeCommandForASM( char** ptrOnSymbolInPosition, FILE* fileForASM ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );
    assert( fileForASM );

    
}
