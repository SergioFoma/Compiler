#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"
#include "dataForBackend.h"

#define FILE_WITH_TREE "commonFiles/AST.txt"     // hardcoding, because the user should not know about the intermediate files.

expertSystemErrors writeAST( tree_t* tree ){
    assert( tree );

    FILE* fileForTree = fopen( FILE_WITH_TREE, "w" );
    if( fileForTree == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in file\n" );
        return ERROR_WITH_FILE;
    }

    dumpTreeInFile( tree, fileForTree );

    fclose( fileForTree );
    return CORRECT_WORK;
}
