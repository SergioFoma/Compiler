#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"
#include "dataForBackend.h"

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
