#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"
#include "dataForBackend.h"

expertSystemErrors writeAST( tree_t* tree, const char* fileNameWithAST ){
    assert( tree );

    FILE* fileForTree = fopen( fileNameWithAST, "w" );
    if( fileForTree == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in file\n" );
        return ERROR_WITH_FILE;
    }

    dumpTreeInFile( tree, fileForTree );

    fclose( fileForTree );
    return CORRECT_WORK;
}
